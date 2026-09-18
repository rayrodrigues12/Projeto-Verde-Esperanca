const http = require("http");
const fs = require("fs");
const path = require("path");
const { execFile } = require("child_process");

const PORT = 3000;

const publicDir = path.join(__dirname, "public");

const cProgram = path.join(
    __dirname,
    "cprogram",
    "processamento.exe"
);


const mimeTypes = {

    ".html": "text/html; charset=UTF-8",

    ".css": "text/css; charset=UTF-8",

    ".js": "application/javascript; charset=UTF-8",

    ".png": "image/png",

    ".jpg": "image/jpeg",

    ".jpeg": "image/jpeg",

    ".gif": "image/gif"

};


const server = http.createServer((req, res) => {

    /*
     * GET
     * Used to request the HTML, CSS and JavaScript files.
     */

    if (req.method === "GET") {

        let urlPath = req.url.split("?")[0];

        if (urlPath === "/") {
            urlPath = "/index.html";
        }

        const filePath = path.join(
            publicDir,
            decodeURIComponent(urlPath)
        );


        fs.readFile(filePath, (error, data) => {

            if (error) {

                res.writeHead(404, {
                    "Content-Type":
                        "text/plain; charset=UTF-8"
                });

                res.end("Página não encontrada.");

                return;
            }


            const extension =
                path.extname(filePath).toLowerCase();

            const contentType =
                mimeTypes[extension] ||
                "application/octet-stream";


            res.writeHead(200, {
                "Content-Type": contentType
            });

            res.end(data);

        });

        return;
    }


    /*
     * POST /pagto
     *
     * Receives:
     * codigo
     * valor
     * tipo
     */

    if (req.method === "POST" &&
        req.url === "/pagto") {

        let body = "";


        req.on("data", chunk => {

            body += chunk.toString();

        });


        req.on("end", () => {

            const params = new URLSearchParams(body);


            const codigo = params.get("codigo");

            const valor = params.get("valor");

            const tipo = params.get("tipo");


            console.log("Código:", codigo);

            console.log("Valor:", valor);

            console.log("Tipo:", tipo);


            /*
             * Validate code
             */

            if (!/^\d{6}$/.test(codigo)) {

                res.writeHead(400, {
                    "Content-Type":
                        "text/plain; charset=UTF-8"
                });

                res.end(
                    "Código deve possuir exatamente 6 dígitos."
                );

                return;
            }


            /*
            * Validate value
            * O valor recebido aqui ja esta em centavos.
            * R$ 10,00 = 1000
            */

            if (!valor || !Number.isInteger(Number(valor)) || Number(valor) < 1000) {

            res.writeHead(400, {
            "Content-Type":
            "text/plain; charset=UTF-8"
            });

            res.end("O valor minimo e R$ 10,00.");

            return;
            }


            /*
             * Validate payment type
             */

            if (!["0", "1", "2"].includes(tipo)) {

                res.writeHead(400, {
                    "Content-Type":
                        "text/plain; charset=UTF-8"
                });

                res.end(
                    "Tipo de pagamento inválido."
                );

                return;
            }


            /*
             * Send the three values
             * to the C program.
             *
             * argc / argv:
             *
             * argv[1] = codigo
             * argv[2] = valor
             * argv[3] = tipo
             */

            execFile(
                cProgram,
                [codigo, valor, tipo],
                (error, stdout, stderr) => {

                    if (error) {

                        console.error(
                            "Erro ao executar C:"
                        );

                        console.error(error);

                        res.writeHead(500, {
                            "Content-Type":
                                "text/plain; charset=UTF-8"
                        });

                        res.end(
                            "Erro ao executar o programa C."
                        );

                        return;
                    }


                    console.log(
                        "Resposta do C:"
                    );

                    console.log(stdout);


                    res.writeHead(200, {
                        "Content-Type":
                            "text/html; charset=UTF-8"
                    });


                    res.end(`
                        <!DOCTYPE html>

                        <html lang="pt-BR">

                        <head>

                            <meta charset="UTF-8">

                            <title>Resultado</title>

                        <link
                             rel="stylesheet"
                             href="/doacao/css/style.css"
                        >

                        </head>

                        <body>

                            <header>
                                <h1>Resultado</h1>
                            </header>

                            <main>

                                <section class="content">

                                    <h2>
                                        Pagamento processado
                                    </h2>

                                    <p>
                                        <strong>Código:</strong>
                                        ${codigo}
                                    </p>

                                    <p>
                                        <strong>Valor:</strong>
                                        ${valor}
                                    </p>

                                    <p>
                                        <strong>Tipo:</strong>
                                        ${tipo}
                                    </p>

                                    <h3>
                                        Resposta do programa C:
                                    </h3>

                                    <pre>${stdout}</pre>

                                    <a
                                        class="button"
                                        href="/doacao/pagamento.html"
                                    >
                                        Fazer nova doação
                                    </a>

                                    <a
                                        class="button"
                                        href="/"
                                    >
                                        Voltar ao site
                                    </a>

                                </section>

                            </main>

                        </body>

                        </html>
                    `);

                }
            );

        });

        return;
    }


    /*
     * Unknown route
     */

    res.writeHead(404, {
        "Content-Type":
            "text/plain; charset=UTF-8"
    });

    res.end("Página não encontrada.");

});


server.listen(PORT, () => {

    console.log(
        `Servidor executando em http://localhost:${PORT}`
    );

});
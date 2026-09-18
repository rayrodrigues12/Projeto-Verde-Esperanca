const form = document.getElementById("paymentForm");

if (form) {

    form.addEventListener("submit", function(event) {

        const codigo = document.getElementById("codigo").value;
        const campoValor = document.getElementById("valor");

        const valorReais = Number(campoValor.value);

        const tipo = document.querySelector(
            'input[name="tipo"]:checked'
        );

        const message = document.getElementById("message");

        /* Valida codigo de 6 digitos */
        if (!/^\d{6}$/.test(codigo)) {

            event.preventDefault();

            message.textContent =
                "O código deve possuir exatamente 6 números.";

            return;
        }

        /* Valor minimo de R$ 10,00 */
        if (
            campoValor.value === "" ||
            !Number.isFinite(valorReais) ||
            valorReais < 10
        ) {

            event.preventDefault();

            message.textContent =
                "O valor mínimo é R$ 10,00.";

            return;
        }

        /* Verifica o meio de pagamento */
        if (!tipo) {

            event.preventDefault();

            message.textContent =
                "Selecione o tipo de pagamento.";

            return;
        }

        /*
         * Converte reais para centavos.
         *
         * Exemplo:
         * R$ 100,00 -> 10000
         */

        const valorCentavos = Math.round(valorReais * 100);

        campoValor.value = valorCentavos;

        console.log("Código:", codigo);
        console.log("Valor em centavos:", valorCentavos);
        console.log("Tipo:", tipo.value);

    });
}

public class LogTrace {
    OperationKind kind;
    IToyProduct oldOne;
    IToyProduct newOne;

    // a record of a cmd
    public LogTrace(OperationKind op, IToyProduct oldProduct, IToyProduct newOne) {
        assert op == OperationKind.Purchase || op == OperationKind.Sell;
        this.kind = op;
        this.oldOne = oldProduct;
        this.newOne = newOne;
    }

    public LogTrace(OperationKind op, IToyProduct newOne) {
        assert op == OperationKind.Create;  // only in create-toy kind could use this constructor
        this.kind = op;
        this.newOne = newOne;
        this.oldOne = null;
    }

    @Override
    public String toString() {
        String ret;
        switch (this.kind) {
            case Create -> ret = "Create:\n Info: " + newOne.getName();

            case Sell -> ret = "Sell:\n Info: From" + oldOne + " To: " + newOne;

            case Purchase -> ret = "Purchase:\n Info: From" + oldOne + " To: " + newOne;

            // unreachable code
            default -> ret = "";
        }
        return ret;
    }
}

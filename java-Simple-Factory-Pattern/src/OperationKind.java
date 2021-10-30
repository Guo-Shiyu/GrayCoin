public enum OperationKind {
    Sell, Purchase, Create;

    public String toString() {
        String ret;
        switch (this) {
            case Sell -> ret = "Sell";
            case Purchase -> ret = "Purchase";
            case Create -> ret = "Create";

            // unreachable code
            default -> ret = "unknown";
        }
        return ret;
    }
}

public class LogTrace {
    String log;

    public LogTrace(OperationKind op, ToyProduct product) {
        this.log = op.toString() + product.toString();
    }

    @Override
    public String toString() {
        return log;
    }
}

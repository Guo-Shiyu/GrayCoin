import java.util.ArrayList;

public class CmdStack {
    private ArrayList<LogTrace> inner;

    public CmdStack() {
        this.inner = new ArrayList<>();
    }

    public void push(LogTrace lt) {
        this.inner.add(lt);
    }

    public LogTrace pop() {
        return this.inner.remove(this.inner.size() - 1);
    }

    public ArrayList<LogTrace> getStack() {
        return this.inner;
    }
}

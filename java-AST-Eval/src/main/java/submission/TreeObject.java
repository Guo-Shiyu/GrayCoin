package submission;

import java.util.ArrayList;

public class TreeObject {
    private TreeObject parent;
    private TreeObject leftChild;
    private TreeObject rightSibling;
    private ArrayList<TreeObject> children; // 模拟单链表， 连接同级兄弟节点
    private String data;

    static final String UNMEANING = ".";

    TreeObject()
    {
        parent = null;
        leftChild = null;
        rightSibling = null;
        children = new ArrayList<>();
        for (int i = 0; i < 5; i++) {
            children.add(null);
        }
        data = UNMEANING;
    }

    TreeObject(String data)
    {
        parent = null;
        leftChild = null;
        rightSibling = null;
        children = new ArrayList<>();
        for (int i = 0; i < 5; i++) {
            children.add(null);
        }
        this.data = data;
    }

    boolean isInitialized()
    {
        return data != UNMEANING;
    }

    public TreeObject getParent() {
        return parent;
    }

    public void setData(String data) {
        this.data = data;
    }

    public void setParent(TreeObject parent) {
        this.parent = parent;
    }

    public TreeObject getLeftChild() {
        return leftChild;
    }

    public void setLeftChildData(String data) {
        // safe
        if (this.leftChild == null ) {
            this.leftChild = new TreeObject(data);
        }else {
            this.leftChild.setData(data);
        }
    }

    public TreeObject getRightSibling() {
        return rightSibling;
    }

    public void setLeftChild(TreeObject leftChild) {
        this.leftChild = leftChild;
    }

    public void setRightSibling(TreeObject rightSibling) {
        this.rightSibling = rightSibling;
    }

    public void setRightSiblingData(String data) {
        // safe
        if (this.rightSibling == null) {
            this.rightSibling = new TreeObject(data);
        } else {
            this.rightSibling.setData(data);
        }
    }

    public ArrayList<TreeObject> getChildren() {
        return children;
    }

    public String getData() {
        return data;
    }

    @Override
    public String toString() {
        return "TreeObject{" +
                "parent=" + parent +
                ", leftChild=" + leftChild +
                ", rightSiblings=" + rightSibling +
                ", data='" + data + '\'' +
                '}';
    }

    public void pushChild(TreeObject object) {
        this.children.add(object);
    }

    public boolean isOperator() {
        return checkOperator(this.data);
    }

    public static  boolean checkOperator(String str) {
        return str.equals("~") ||
                str.equals("?") ||
                str.equals("&");
    }

    public boolean isNumber() {
        return !isOperator();
    }

    public double value() {
        return Double.parseDouble(data);
    }
}

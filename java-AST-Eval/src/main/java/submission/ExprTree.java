package submission;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Iterator;
import java.util.Objects;

public class ExprTree implements Iterable<TreeObject> {
    private TreeObject root;

    public ExprTree() {
        this.root = new TreeObject();
    }

    @Override
    public String toString() {
        return "ExprTree{" +
                "root=" + root +
                '}';
    }

    private TreeObject parseNode(Iterator<String> iter, TreeObject now) {
        assert iter.hasNext();
        if (now == null) {
            now = new TreeObject(iter.next());
        }else {
            now.setData(iter.next());
        }
        switch (now.getData()) {
            case "~":
                for (int i = 0; i < 3; i++) {
                    var children = now.getChildren();
                    children.add(i, parseNode(iter, children.get(i)));
                }
                break;
            case "?":
                for (int i = 0; i < 4; i++) {
                    var children = now.getChildren();
                    children.add(i, parseNode(iter, children.get(i)));
                }
                break;
            case "&":
                for (int i = 0; i < 5; i++) {
                    var children = now.getChildren();
                    children.add(i, parseNode(iter, children.get(i)));
                }
                break;
        }
        return now;
    }

    private void contactLeftAndRight(TreeObject node) {
        var children = node.getChildren();
        children.removeIf(Objects::isNull);
        if (children.size() > 0) {
            assert children.size() >= 2;
            node.setLeftChild(children.get(0));
            node.setRightSibling(children.get(1));
            for (var child :
                    children) {
                contactLeftAndRight(child);
            }
        }
    }

    public ExprTree rawParse(String raw) throws Exception {
        var now = root;
        var seq = Arrays.stream(raw.split(" ")).iterator();
        root = parseNode(seq, now);

        TreeObject ref = root;
        contactLeftAndRight(ref);

        return this;
    }

    interface TreeVisitor<R> {
        R dealNode(TreeObject object);
    }

    private void visit(TreeVisitor v) {
        for (TreeObject object : this) {
            v.dealNode(object);
        }
    }

    private String toNormalNode(TreeObject node) {
        StringBuilder ret = new StringBuilder();
        var value = node.getData();
        if (node.isOperator()) {
            ret.append("( ");

            var children = node.getChildren();
            // a + b - c
            if (value.equals("~")) {
                ret.append(toNormalNode(children.get(0)))
                        .append(" + ")
                        .append(toNormalNode(children.get(1)))
                        .append(" - ")
                        .append(toNormalNode(children.get(2)));
            } else if (value.equals("&")) {
                // a + b + c - d - e
                ret.append(toNormalNode(children.get(0)))
                        .append(" + ")
                        .append(toNormalNode(children.get(1)))
                        .append(" + ")
                        .append(toNormalNode(children.get(2)))
                        .append(" - ")
                        .append(toNormalNode(children.get(3)))
                        .append(" - ")
                        .append(toNormalNode(children.get(4)));
            } else if (value.equals("?")) {
                // a + b - c + d
                ret.append(toNormalNode(children.get(0)))
                        .append(" + ")
                        .append(toNormalNode(children.get(1)))
                        .append(" - ")
                        .append(toNormalNode(children.get(2)))
                        .append(" + ")
                        .append(toNormalNode(children.get(3)));
            }

            ret.append(" )");
        }else {
            ret.append(value);
        }
        return ret.toString();
    }

    public String normalExpr() {
        return toNormalNode(root);
    }

    private double evalNode(TreeObject node) {
        if (node.isNumber()) {
            return node.value();
        } else {
            var op = node.getData();
            var children = node.getChildren();
            if (op.equals("~")) {
                return  evalNode(children.get(0)) + evalNode(children.get(1)) - evalNode(children.get(2));
            } else if (op.equals("?")) {
                return evalNode(children.get(0)) + evalNode(children.get(1)) - evalNode(children.get(2)) + evalNode((children.get(3)));
            } else if (op.equals("&")) {
                return evalNode(children.get(0)) + evalNode(children.get(1)) + evalNode(children.get(2)) - evalNode((children.get(3))) - evalNode(children.get(4));
            } else {
                // unreachable code
                return 42.42;
            }
        }
    }

    public double eval() {
        return evalNode(root);
    }

    @Override
    public Iterator<TreeObject> iterator() {
        return new Iterator<TreeObject>() {
            private TreeObject now = root;
            private int index;

            @Override
            public boolean hasNext() {
                if (index < now.getChildren().size()) {
                    return true;
                } else {
                    if (now.getParent() == null) {
                        return  false;
                    }else {
                        index = now.getParent().getChildren().indexOf(now) + 1;
                        now = now.getParent();
                        return hasNext();
                    }
                }
            }

            @Override
            public TreeObject next() {
                return now;
            }
        };
    }
}

package submission;

import java.awt.Color;
import java.awt.Font;
import java.awt.Graphics;

import javax.swing.JPanel;

public class TreePanel extends JPanel {

    private ExprTree tree;              //保存整棵树
    private int gridWidth = 40;     //每个结点的宽度
    private int gridHeight = 40;    //每个结点的高度
    private int vGap = 50;          //每2个结点的垂直距离  
    private int hGap = 30;          //每2个结点的水平距离  

    private int startY = 10;        //根结点的Y，默认距离顶部10像素  
    private int startX = 0;         //根结点的X，默认水平居中对齐  

    private int childAlign;                     //孩子对齐方式  
    public static int CHILD_ALIGN_ABSOLUTE = 0; //相对Panel居中  
    public static int CHILD_ALIGN_RELATIVE = 1; //相对父结点居中  

    private Font font = new Font("微软雅黑", Font.BOLD, 14);  //描述结点的字体

    private Color gridColor = Color.WHITE;      //结点背景颜色
    private Color linkLineColor = Color.blue;  //结点连线颜色
    private Color stringColor = Color.BLACK;    //结点描述文字的颜色
    private Color borderColor = Color.blue; //结点边框的颜色

    private int layer;  //记录当前深度的值， 用于绘图

    public TreePanel(int childAlign) {
        this(null, childAlign);
        layer = 0;
    }

    public TreePanel(ExprTree n, int childAlign) {
        super();
        setTree(n);
        this.childAlign = childAlign;
        layer = 0;
    }
    public void setTree(ExprTree n) {
        tree = n;
    }

    public void paintComponent(Graphics g) {
        startX = (getWidth() - gridWidth) / 2 - getWidth() / 5;
        super.paintComponent(g);
        g.setFont(font);
        drawAllNode(tree.iterator().next(), startX, g);
    }

    /**
     * 递归绘制整棵树
     *
     * @param n 被绘制的Node
     * @param x 根节点的绘制X位置
     * @param g 绘图上下文环境
     */


    public void drawAllNode(TreeObject n, int x, Graphics g) {
        int y = layer * (vGap + gridHeight) + startY;
        int fontY = y + gridHeight - 5;

        g.setColor(borderColor);
        g.drawRect(x - 1, y - 1, gridWidth + 1, gridHeight + 1); //画边框
        g.setColor(gridColor);
        g.fillRect(x, y, gridWidth, gridHeight);    //画结点的格子

        g.setColor(stringColor);
        g.drawString(n.getData(), x + 10, fontY);       //画结点的名字

        var children = n.getChildren();
        if (children.size() != 0) {
            int size = children.size();
            int tempPosx = childAlign == CHILD_ALIGN_RELATIVE
                    ? x + gridWidth / 2 - (size * (gridWidth + hGap) - hGap) / 2
                    : (getWidth() - size * (gridWidth + hGap) + hGap) / 2;

            int i = 0;
            layer++;    // 绘制孩子， 层次增加
            for (int j = 0; j < children.size(); j++) {
                int newX = tempPosx + (gridWidth + hGap) * i; //孩子结点起始X
                g.setColor(linkLineColor);
                g.drawLine(x + gridWidth / 2, y + gridHeight, newX + gridWidth / 2, y + gridHeight + vGap);   //画连接结点的线
                drawAllNode(children.get(j), newX, g);
                i++;
            }
            layer--;    // 结束绘制， 层次回复
        }
    }


    public Color getGridColor() {
        return gridColor;
    }

    public void setGridColor(Color gridColor) {
        this.gridColor = gridColor;
    }

    public Color getLinkLineColor() {
        return linkLineColor;
    }

    public void setLinkLineColor(Color gridLinkLine) {
        this.linkLineColor = gridLinkLine;
    }

    public Color getStringColor() {
        return stringColor;
    }

    public void setStringColor(Color stringColor) {
        this.stringColor = stringColor;
    }

    public int getStartY() {
        return startY;
    }

    public void setStartY(int startY) {
        this.startY = startY;
    }

    public int getStartX() {
        return startX;
    }
    public void setStartX(int startX) {
        this.startX = startX;
    }

    public Color getBorderColor() {
        return borderColor;
    }

    public void setBorderColor(Color borderColor) {
        this.borderColor = borderColor;
    }
}
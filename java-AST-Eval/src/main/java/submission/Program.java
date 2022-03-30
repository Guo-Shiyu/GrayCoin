package submission;

import javax.swing.*;
import javax.swing.border.EmptyBorder;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

/**
 * @author John
 */
public class Program extends JFrame {

    public Program() {
        super("Tree");
        initComponents();
    }

    public static void main(String[] args) {
        Program frame = new Program();

        frame.setSize(800, 600);
        frame.setLocationRelativeTo(null);
        frame.setVisible(true);
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
    }

    public void initComponents() {
        //   ? 1 2 ~ -7 6 5 4
        final boolean[] flag = {false};
        JPanel contentPane2 = new JPanel();
        JTextField field = new JTextField("~ 3 2 ~ -2 5 7", 25);
        field.setPreferredSize(new Dimension(20, 30));
        contentPane2.add(field);
        JButton eval = new JButton("eval");
        eval.setPreferredSize(new Dimension(70, 30));
        contentPane2.setBorder(new EmptyBorder(10, 0, 0, 0));
        contentPane2.add(eval);
        final JLabel[] result = {new JLabel()};
        contentPane2.add(result[0]);
        eval.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                TreePanel panel1 = new TreePanel(TreePanel.CHILD_ALIGN_RELATIVE);
                try {
                    String text = field.getText();
                    System.out.println(text);
                    //构建树形结构
                    ExprTree tree = new ExprTree();
                    tree.rawParse(text);

                    //解析为正常的表达式
                    String expression = tree.normalExpr();

                    //计算结果
                    double eval = tree.eval();
                    System.out.println(eval);
                    System.out.println(expression);

                    //解析后的表达式的显示
                    result[0].setText(expression + "=" + eval);

                    //树图的显示
                    panel1.setTree(tree);
                    JPanel contentPane = new JPanel();
                    contentPane.setLayout(new CardLayout());
                    contentPane.add(panel1);
                    add(contentPane, BorderLayout.CENTER);
                    refresh();
                } catch (Exception exception) {
                    System.out.println(exception);
                    dig();
                }

            }
        });

        add(contentPane2, BorderLayout.NORTH);

    }

    public void dig() {
        JOptionPane.showConfirmDialog(this, "输入式子有误，请检查后输入！技术qq:2821006329， 有问题直接联系不走中介"
                , "警告", JOptionPane.ERROR_MESSAGE);
    }

    public void refresh() {
        this.revalidate();
    }
}  
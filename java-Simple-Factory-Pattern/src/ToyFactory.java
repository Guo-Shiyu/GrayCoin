import java.util.ArrayList;
import java.util.Locale;
import java.util.Scanner;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class ToyFactory {
    private final static String TextToDisplay = "Toy Inventory Management System (TIMS)\n Please input command:[c|d|p|s|u|r|l|x]\n";
    private final static Scanner scan = new Scanner(System.in);

    private ArrayList<ToyProduct> allProduct;

    private ArrayList<LogTrace> undo;
    private ArrayList<LogTrace> redo;

    private ToyProduct produce(ToyKind type, String id, String name, String heightOrSpeed) {
        ToyProduct ret;
        switch (type) {
            case Robot -> ret = new Robot(Integer.parseInt(id), name, Double.parseDouble(heightOrSpeed));
            case RemoteControlCar -> ret = new RemoteControlCar(Integer.parseInt(id), name, Double.parseDouble(heightOrSpeed));

            // unreachable code
            default -> ret = null;
        }
        return ret;
    }

    private void createToy() {
        System.out.println("Enter toy type: (ro=Robot / rc= Remote Control Car)");
        ToyKind type = ToyKind.fromString(scan.nextLine().toLowerCase(Locale.ROOT).trim());
        switch (type) {
            case Robot -> {
                System.out.println("Enter id, name and height");
                String line = scan.nextLine();
                String reg = "(\\d+),(.*),(-?\\d+)(\\.\\d+)?";
                Matcher result = Pattern.compile(reg).matcher(line);
                System.out.println("Echo: " + line);
                if (result.matches()) {
                    ToyProduct product = this.produce(ToyKind.Robot, result.group(1), result.group(2), result.group(3));
                    this.allProduct.add(product);
                    this.undo.add(new LogTrace(OperationKind.Create, product));
                } else {
                    System.out.println("Invalid input:" + line);
                }

            }

            case RemoteControlCar -> {
                System.out.println("Enter id, name and max speed");
                String line = scan.nextLine().trim();
                String reg = "(\\d+),(.*),(-?\\d+)(\\.\\d+)?";
                Matcher result = Pattern.compile(reg).matcher(line);
                System.out.println("Echo: " + line);
                if (result.matches()) {
                    ToyProduct product = this.produce(ToyKind.RemoteControlCar, result.group(1), result.group(2), result.group(3));
                    this.allProduct.add(product);
                    this.undo.add(new LogTrace(OperationKind.Create, product));
                } else {
                    System.out.println("Invalid input:" + line);
                }
            }

            default -> System.out.println("Invalid input, please input again");
        }
    }

    private void displayToy() {
        System.out.println("Enter id (* to display all)");
        String line = scan.nextLine().trim();
        System.out.println("Echo:"+line);
        if (line.equals("*")) {
            for (ToyProduct p : this.allProduct) {
                System.out.println(p.toString());
            }
        } else {
            boolean flag = false;
            for (ToyProduct p : this.allProduct) {
                if (p.getProductID() == Integer.parseInt(line)) {
                    System.out.println(p.toString());
                    flag = true;
                }
            }
            if (!flag) {
                System.out.println("There is no product with id ="+ line);
            }
        }
    }

    private void purchaseToy() {

    }

    private void sellToy() {

    }

    private void undoLast() {

    }

    private void redoLast() {

    }

    private void listAll() {

    }

    public void loop() {
        String input;
        do {
            System.out.println(TextToDisplay);
            input = scan.nextLine().toLowerCase(Locale.ROOT).trim();
            if (input.equals("c")) {
                this.createToy();
            } else if (input.equals("d")) {
                this.displayToy();
            } else if (input.equals("p")) {
                this.purchaseToy();
            } else if (input.equals("s")) {
                this.sellToy();
            } else if (input.equals("u")) {
                this.undoLast();
            } else if (input.equals("r")) {
                this.redoLast();
            } else if (input.equals("l")) {
                this.listAll();
            } else {
                System.out.println("Unknown command, please input command again");
            }
        } while (!input.equals("x"));
        System.out.println("Exit ...");
    }
}

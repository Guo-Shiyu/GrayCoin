import java.util.ArrayList;
import java.util.Locale;
import java.util.Scanner;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

// main factory class
public class Tims {
    private final static String TextToDisplay = "Toy Inventory Management System (TIMS)\n Please input command:[c|d|p|s|u|r|l|x]\n";
    private final static Scanner scan = new Scanner(System.in);

    // product list
    private ToyFactory factory;

    // command stack for undo and redo list
    private CmdStack undo;
    private CmdStack redo;

    public Tims() {
        this.factory = new ToyFactory();
        this.undo = new CmdStack();
        this.redo = new CmdStack();
    }

    private void createToy() {
        System.out.println("Enter toy type: (ro=Robot / rc= Remote Control Car)");
        ToyKind type = ToyKind.fromString(scan.nextLine().toLowerCase(Locale.ROOT).trim());
        boolean success = false;
        switch (type) {
            case Robot -> {
                System.out.println("Enter id, name and height");
                String line = scan.nextLine();
                String reg = "(\\d+),(.*),(-?\\d+)(\\.\\d+)?";
                Matcher result = Pattern.compile(reg).matcher(line);
                System.out.println("Echo: " + line);
                if (result.matches()) {
                    IToyProduct product = this.factory.produce(ToyKind.Robot, result.group(1), result.group(2), result.group(3));
                    this.factory.addNewProduct(product);
                    this.undo.push(new LogTrace(OperationKind.Create, product));
                    success = true;
                } else {
                    System.out.println("Invalid input:" + line + " please input again");
                }
            }

            case RemoteControlCar -> {
                System.out.println("Enter id, name and max speed");
                String line = scan.nextLine().trim();
                String reg = "(\\d+),(.*),(-?\\d+)(\\.\\d+)?";
                Matcher result = Pattern.compile(reg).matcher(line);
                System.out.println("Echo: " + line);
                if (result.matches()) {
                    IToyProduct product = this.factory.produce(ToyKind.RemoteControlCar, result.group(1), result.group(2), result.group(3));
                    this.factory.addNewProduct(product);
                    this.undo.push(new LogTrace(OperationKind.Create, product));
                    success = true;
                } else {
                    System.out.println("Invalid input:" + line + " please input again");
                }
            }

            default -> System.out.println("Invalid input, please input again");
        }
        if (success) {
            System.out.println("Create toy successfully");
        }
    }

    private void displayToy() {
        System.out.println("Enter id (* to display all)");
        String line = scan.nextLine().trim();
        System.out.println("Echo:"+line);
        System.out.println("ID\tName\tQuantity\tCost\tPrice\tOtherInfo");
        if (line.equals("*")) {
            for (IToyProduct p : this.factory.getAllProduct()) {
                System.out.println(p.lineFormat());
            }
        } else {
            boolean flag = false;
            for (IToyProduct p : this.factory.getAllProduct()) {
                if (p.getProductID() == Integer.parseInt(line)) {
                    System.out.println(p.lineFormat());
                    flag = true;
                }
            }
            if (!flag) {
                System.out.println("There is no product with id ="+ line);
            }
        }
    }

    private void purchaseToy() throws CloneNotSupportedException {
        System.out.println("Enter code:");
        String code = scan.nextLine().trim();
        System.out.println("Quantity to be purchased:");
        String quantity = scan.nextLine();
        System.out.println("Purchasing cost:");
        String cost = scan.nextLine();
        boolean findFlag = false;
        IToyProduct target = null;
        IToyProduct ref = null;
        for (IToyProduct pd : this.factory.getAllProduct()) {
            String curState = pd.toString();
            if (curState.contains(code)) {
                findFlag = true;
                target = pd.clone();
                ref = pd;
                pd.setQty(pd.getQty() + Integer.parseInt(quantity.trim()));
                pd.setCost(pd.getCost() + Double.parseDouble(cost.trim()));
                this.undo.push(new LogTrace(OperationKind.Purchase, target, pd.clone()));
            }
        }
        if (!findFlag) {
            System.out.println("Can not find the product with code = " + code + ", please retry");
        }else {
            System.out.println("Purchase " + quantity + " boxes of " + ref.getName() + ".");
            System.out.println("Current quantity is " + ref.getQty() + " , cost is " + ref.getCost() + " price is " + ref.getPrice());
        }
    }

    private void sellToy() {
        System.out.println("Enter code:");
        String code = scan.nextLine().trim();
        System.out.println("Quantity to be sold:");
        String quantity = scan.nextLine();
        System.out.println("Selling price:");
        String price = scan.nextLine();
        boolean findFlag = false;
        boolean validFlag = true;
        IToyProduct target = null;
        IToyProduct ref = null;
        for (IToyProduct pd : this.factory.getAllProduct()) {
            String curState = pd.toString();
            if (curState.contains(code)) {
                findFlag = true;
                target = pd.clone();
                ref = pd;
                int newQuantity = pd.getQty() - Integer.parseInt(quantity.trim());
                if (newQuantity > 0) {
                    pd.setQty(newQuantity);
                    pd.setPrice(pd.getPrice() + Double.parseDouble(price.trim()));
                    this.undo.push(new LogTrace(OperationKind.Sell, target, pd.clone()));
                }else {
                    validFlag = false;
                }
            }
        }
        if (!findFlag) {
            System.out.println("Can not find the product with code = " + code + ", please retry");
        }else {
            if (validFlag) {
                System.out.println("Sell " + quantity + " boxes of " + ref.getName() + ".");
                System.out.println("Current quantity is " + ref.getQty() + " , cost is " + ref.getCost() + " price is " + ref.getPrice());
            }else {
                System.out.println("Invalid quantity.(current quantity < selling quantity)");
            }
        }
    }

    private void undoLast() {
        LogTrace last = this.undo.pop();
        switch (last.kind) {
            case Create -> {this.factory.deleteProduct(last.newOne);}

            case Purchase, Sell -> {
                this.factory.deleteProduct(last.newOne);
                this.factory.addNewProduct(last.oldOne);
            }

            // unreachable code
            default -> {}
        }
        this.redo.push(last);
        System.out.println("Undo successfully");
    }

    private void redoLast() {
        LogTrace last = this.redo.pop();
        switch (last.kind) {
            case Create -> {
                this.factory.addNewProduct(last.newOne.clone());
            }

            case Purchase, Sell -> {
                this.factory.deleteProduct(last.newOne);
                this.factory.addNewProduct(last.newOne);
            }

            // unreachable code
            default -> {}
        }
        this.undo.push(last);
        System.out.println("Redo successfully");
    }

    private void listAll() {
        System.out.println("Undo List:");
        if (this.undo.getStack().size() > 0) {
            for(LogTrace trace : this.undo.getStack()) {
                System.out.println(trace.toString() + '\n');
            }
        }else {
            System.out.println("Empty");
        }

        System.out.println("Redo List:");
        if (this.redo.getStack().size() > 0) {
            for(LogTrace trace : this.redo.getStack()) {
                System.out.println(trace.toString() + '\n');
            }
        }else {
            System.out.println("Empty");
        }
    }

    public void loop() {
        String input = null;
        do {
            try {
                System.out.println("====================================");
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
            }catch (CloneNotSupportedException e) {
                e.printStackTrace();
            }
        } while (!input.equals("x"));
        System.out.println("Exit ...");
    }



}

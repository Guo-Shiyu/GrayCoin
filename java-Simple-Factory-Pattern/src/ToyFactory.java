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

    public ToyFactory() {
        this.allProduct = new ArrayList<>();
        this.undo = new ArrayList<>();
        this.redo = new ArrayList<>();
    }

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
        boolean success = false;
        switch (type) {
            case Robot -> {
                success = true;
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
                    System.out.println("Invalid input:" + line + " please input again");
                }
            }

            case RemoteControlCar -> {
                success = true;
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

    private void purchaseToy() throws CloneNotSupportedException {
        System.out.println("Enter code:");
        String code = scan.nextLine().trim();
        System.out.println("Quantity to be purchased:");
        String quantity = scan.nextLine();
        System.out.println("Purchasing cost:");
        String cost = scan.nextLine();
        boolean findFlag = false;
        ToyProduct target = null;
        for (ToyProduct pd : allProduct) {
            String curState = pd.toString();
            if (curState.contains(code)) {
               findFlag = true;
               target = pd.clone();
               pd.setQty(pd.getQty() + Integer.parseInt(quantity.trim()));
               pd.setCost(pd.getCost() + Double.parseDouble(cost.trim()));
               this.undo.add(new LogTrace(OperationKind.Purchase, target, pd.clone()));
            }
        }
        if (!findFlag) {
            System.out.println("Can not find the product with code = " + code + ", please retry");
        }else {
            System.out.println("Purchase " + quantity + " boxes of " + target.getName() + ".");
            System.out.println("Current quantity is " + target.getQty() + " , cost is " + target.getCost() + " price is " + target.getPrice());
        }
    }

    private void sellToy() throws CloneNotSupportedException {
        System.out.println("Enter code:");
        String code = scan.nextLine().trim();
        System.out.println("Quantity to be sold:");
        String quantity = scan.nextLine();
        System.out.println("Selling price:");
        String price = scan.nextLine();
        boolean findFlag = false;
        ToyProduct target = null;
        for (ToyProduct pd : allProduct) {
            String curState = pd.toString();
            if (curState.contains(code)) {
                findFlag = true;
                target = pd.clone();
                pd.setQty(pd.getQty() - Integer.parseInt(quantity.trim()));
                pd.setPrice(pd.getPrice() + Double.parseDouble(price.trim()));
                this.undo.add(new LogTrace(OperationKind.Sell, target, pd.clone()));
            }
        }
        if (!findFlag) {
            System.out.println("Can not find the product with code = " + code + ", please retry");
        }else {
            System.out.println("Sell " + quantity + " boxes of " + target.getName() + ".");
            System.out.println("Current quantity is " + target.getQty() + " , cost is " + target.getCost() + " price is " + target.getPrice());
        }
    }

    private void undoLast() throws CloneNotSupportedException {
        LogTrace last = this.undo.remove(this.undo.size() - 1);
        switch (last.kind) {
            case Create -> {
                for(ToyProduct toy : this.allProduct){
                    if (toy.equals(last.newOne)){
                        this.allProduct.remove(toy);
                        break;
                    }
                }
            }

            case Purchase, Sell -> {
                for(ToyProduct toy : this.allProduct){
                    if (toy.equals(last.newOne)){
                        toy = last.oldOne.clone();
                        break;
                    }
                }
            }

            // unreachable code
            default -> {}
        }
        this.redo.add(last);
        System.out.println("Undo successfully");
    }

    private void redoLast() throws CloneNotSupportedException {
        LogTrace last = this.redo.remove(this.redo.size() - 1);
        switch (last.kind) {
            case Create -> {
                this.allProduct.add((ToyProduct) last.newOne.clone());
            }

            case Purchase, Sell -> {
                for(ToyProduct toy : this.allProduct){
                    if (toy.equals(last.newOne)){
                        toy = last.oldOne;
                        break;
                    }
                }
            }

            // unreachable code
            default -> {}
        }
        this.undo.add(last);
        System.out.println("Redo successfully");
    }

    private void listAll() {
        System.out.println("Undo List:");
        if (this.undo.size() > 0) {
            for(LogTrace trace : this.undo) {
                System.out.println(trace.toString() + '\n');
            }
        }else {
            System.out.println("Empty");
        }

        System.out.println("Redo List:");
        if (this.redo.size() > 0) {
            for(LogTrace trace : this.redo) {
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

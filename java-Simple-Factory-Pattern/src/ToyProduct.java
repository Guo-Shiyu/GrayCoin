import java.util.Formatter;

// product base class
public class ToyProduct implements IToyProduct {
    private int productID;
    private String name;
    private int qty;
    private double cost;
    private double price;

    public ToyProduct(int productID, String name) {
        this.productID = productID;
        this.name = name;
    }

    public String lineFormat() {
        StringBuilder builder = new StringBuilder();
        Formatter fmt = new Formatter(builder);
        fmt.format("%d\t%-4s\t%-4d\t%-4f\t%-4f\t", this.productID, this.name, this.qty, this.cost, this.price);
        return fmt.toString();
    }

    @Override
    public String toString() {
        return "ToyProduct{" +
                "productID=" + productID +
                ", name='" + name + '\'' +
                ", qty=" + qty +
                ", cost=" + cost +
                ", price=" + price +
                '}';
    }

    @Override
    public ToyProduct clone() {
        ToyProduct product = null;
        try {
            product = (ToyProduct) super.clone();
        } catch (CloneNotSupportedException e) {
            product = null;
        }
        return product;
    }

    public int getProductID() {
        return productID;
    }

    public void setProductID(int productID) {
        this.productID = productID;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public int getQty() {
        return qty;
    }

    public void setQty(int qty) {
        this.qty = qty;
    }

    public double getCost() {
        return cost;
    }

    public void setCost(double cost) {
        this.cost = cost;
    }

    public double getPrice() {
        return price;
    }

    public void setPrice(double price) {
        this.price = price;
    }
}

public class Robot extends ToyProduct {
    private double height;

    public Robot(int productID, String name, double height) {
        super(productID, name);
        this.height = height;
    }

    @Override
    public String toString() {
        return "Robot{" +
                "height=" + height +
                ", productID=" + super.getProductID() +
                ", name='" + super.getName() + '\'' +
                ", qty=" + super.getQty() +
                ", cost=" + super.getCost() +
                ", price=" + super.getPrice() +
                '}';
    }

    public double getHeight() {
        return height;
    }

    public void setHeight(double height) {
        this.height = height;
    }
}

public class Robot extends ToyProduct {
    private double height;

    public Robot(int productID, String name, double height) {
        super(productID, name);
        this.height = height;
    }

    public String lineFormat() {
        return super.lineFormat() + this.height + "cm";
    }

    @Override
    public String toString() {
        return "Robot" + " Name='" + super.getName() + '\'' +
                "\nProductID=" + super.getProductID() +
                "\nHeight(cm)=" + height +
                "\nQuantity=" + super.getQty() +
                "\ncost($)=" + super.getCost() +
                "\nprice($)=" + super.getPrice();
    }

    public double getHeight() {
        return height;
    }

    public void setHeight(double height) {
        this.height = height;
    }
}

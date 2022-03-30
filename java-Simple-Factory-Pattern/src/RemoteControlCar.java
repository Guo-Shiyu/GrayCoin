public class RemoteControlCar extends ToyProduct {
    private double maxSpeed;

    public RemoteControlCar(int productID, String name, double maxSpeed) {
        super(productID, name);
        this.maxSpeed = maxSpeed;
    }

    @Override
    public String lineFormat() {
        return super.lineFormat() + this.maxSpeed + "m/s";
    }

    @Override
    public String toString() {
        return "RemoteControlCar " + " name='" + super.getName() + '\'' +
                "\nProductID=" + super.getProductID() +
                "\nMaxSpeed(m/s)=" + maxSpeed +
                "\nQuantity=" + super.getQty() +
                "\nCost($)=" + super.getCost() +
                "\nPrice($)=" + super.getPrice();
    }

    public double getMaxSpeed() {
        return maxSpeed;
    }

    public void setMaxSpeed(double maxSpeed) {
        this.maxSpeed = maxSpeed;
    }
}

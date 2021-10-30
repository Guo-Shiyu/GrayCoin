public class RemoteControlCar extends ToyProduct {
    private double maxSpeed;

    public RemoteControlCar(int productID, String name, double maxSpeed) {
        super(productID, name);
        this.maxSpeed = maxSpeed;
    }

    @Override
    public String toString() {
        return "RemoteControlCar{" +
                "maxSpeed=" + maxSpeed +
                ", productID=" + super.getProductID() +
                ", name='" + super.getName() + '\'' +
                ", qty=" + super.getQty() +
                ", cost=" + super.getCost() +
                ", price=" + super.getPrice() +
                '}';
    }

    public double getMaxSpeed() {
        return maxSpeed;
    }

    public void setMaxSpeed(double maxSpeed) {
        this.maxSpeed = maxSpeed;
    }
}

public interface IToyProduct extends Cloneable {

    public int getProductID();

    public void setProductID(int id);

    public String getName() ;

    public void setName(String name);

    public int getQty();

    public void setQty(int qty);

    public double getCost();

    public void setCost(double cost);

    public double getPrice();

    public void setPrice(double price);

    public String lineFormat();

    @Override
    public String toString();

    public IToyProduct clone();
}

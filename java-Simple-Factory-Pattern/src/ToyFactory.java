import java.util.ArrayList;
import java.util.Locale;
import java.util.Scanner;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

// main factory class
public class ToyFactory {
    // product list
    private ArrayList<IToyProduct> allProduct;

    public ToyFactory() {
        this.allProduct = new ArrayList<>();
    }

    // produce operation
    // dispatch product task by enum
    public IToyProduct produce(ToyKind type, String id, String name, String heightOrSpeed) {
        ToyProduct ret;
        // dispatch produce by toyKind enum
        switch (type) {
            case Robot -> ret = new Robot(Integer.parseInt(id), name, Double.parseDouble(heightOrSpeed));
            case RemoteControlCar -> ret = new RemoteControlCar(Integer.parseInt(id), name, Double.parseDouble(heightOrSpeed));

            // unreachable code
            default -> ret = null;
        }
        return ret;
    }

    public ArrayList<IToyProduct> getAllProduct() {
        return this.allProduct;
    }

    public void addNewProduct(IToyProduct product) {
        this.allProduct.add(product);
    }

    public void deleteProduct(IToyProduct product) {
        for(IToyProduct toy : this.allProduct){
            if (toy.equals(product)){
                this.allProduct.remove(toy);
                break;
            }
        }
    }


}

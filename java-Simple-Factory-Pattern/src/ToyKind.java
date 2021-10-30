import java.util.Locale;

public enum ToyKind {
    RemoteControlCar, Robot;

    public String toString() {
        String ret;
        switch (this) {
            case Robot -> ret = "ro";
            case RemoteControlCar -> ret = "rc";

            // unreachable code
            default -> ret = "unknown";
        }
        return ret;
    }

    public static ToyKind fromString(String str) {
        String toCompare = str.toLowerCase(Locale.ROOT);
        if (toCompare.equals("rc") || toCompare.equals("RemoteControlCar".toLowerCase(Locale.ROOT))) {
            return ToyKind.RemoteControlCar;
        } else if (toCompare.equals("ro") || toCompare.equals("robot")) {
            return ToyKind.Robot;
        } else {
            return null;
        }
    }
}

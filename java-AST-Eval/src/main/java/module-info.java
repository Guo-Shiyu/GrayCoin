module com.example.javaasteval {
    requires javafx.controls;
    requires javafx.fxml;
    requires java.desktop;


    opens com.example.javaasteval to javafx.fxml;
    exports com.example.javaasteval;
}
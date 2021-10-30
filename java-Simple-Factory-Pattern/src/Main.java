import java.util.Scanner;
public class Main {

    public static void main(String[] args) {
        boolean repeat = true;
        Scanner scan = new Scanner(System.in);
        while(repeat) {
            System.out.println("please input some letters:");
            String buf = scan.nextLine();
            int[] frequency = new int[26];
            for(int i = 0; i< buf.length(); i++) {
                char next = buf.charAt(i);
                // System.out.println("echo:" + (char)next);
                if ( next >= 'a' && next <= 'z') {
                    int index = next - 'a';
                    frequency[index]++;
                }

                if ( next >= 'A' && next <= 'Z') {
                    int index = next - 'A';
                    frequency[index]++;
                }
            }
            System.out.println("frequency of a-z is:");
            for (int i = 0; i < 26; i++) {
                System.out.println((char)('a' + i) + " : " + frequency[i]);
            }

            System.out.println("would you want to try again? (y/n)");
            String ansuer = scan.nextLine();
            if (ansuer.equals("y") || ansuer.equals("Y")) {
                repeat = true;
            } else {
                repeat = false;
            }
        }

        System.out.println("goodbye!");
    }
}

/*
ToyFactory factory = new ToyFactory();
        factory.loop();
*/

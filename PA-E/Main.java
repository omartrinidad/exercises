
import java.io.IOException;
import java.io.File;
import java.util.Scanner;

public class Main {

    public static double[][] readFile(String fileName) {

        double[][] patterns;
        int M = 1000;
        int N = 2;
        patterns = new double[M][N];

        try {
            Scanner scanner = new Scanner(new File(fileName));

            int m = 0;
            while (scanner.hasNext()){

                /* Warning here: two spaces */
                String[] values = scanner.nextLine().split("  ");

                for (int n = 0; n < N; n++) {
                    patterns[m][n] = Double.parseDouble(values[n]);
                    System.out.print( patterns[m][n] );
                }

                m += 1;
            }

        } catch (IOException e) {
            System.out.println(e.getMessage());
        }

        return patterns;

    }

    public static void main(String[] args) {

        double[][] patterns;
        patterns = readFile(args[0]);

        /* Patterns */
        int M = 4;
        int N = 2;
        int K = 20;

        /* setting area */
        MultiGas multiGas = new MultiGas(M, N, K);
        multiGas.initCenters(patterns);

    }

}

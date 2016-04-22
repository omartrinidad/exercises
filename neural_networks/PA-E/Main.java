
import java.io.IOException;
import java.io.File;
import java.io.FileNotFoundException;
import java.util.Scanner;
import java.util.concurrent.ThreadLocalRandom;

public class Main {

    public static double[][] readFile(String fileName) {
        /*
         * ToDo: Create an exception for the size of the array
         * */

        double[][] patterns;
        //int M = 1000;
        int M = 3;
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
                }

                m += 1;
            }

        } catch (FileNotFoundException e) {
            System.out.println("The filename you provided it was not found");
            System.exit(0);
        }

        return patterns;

    }

    public static void main(String[] args) {

        /* Patterns */
        double[][] patterns;
        patterns = readFile(args[0]);

        /* setting area */
        int M = 3;
        int N = patterns[0].length;
        int K = 5;
        MultiGas multiGas = new MultiGas(M, N, K);

        //multiGas.initCenters(patterns); 

        /* training */

        int epoch = 100;
        for (int e = 1; e <= epoch ; e++) {

            /*
             * ToDo: randomize elements of array
             * int index = ThreadLocalRandom.current().nextInt(1, M+1);
            */
            
            for (int p = 0; p < patterns.length; p++ ) {
                multiGas.applyStimulus( patterns[p] );
            }
                
        }

    }

}

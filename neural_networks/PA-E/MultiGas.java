
class Neuron {

    private double[] center;

    public Neuron(int N) {

        center = new double[N];

        /* Initialize Ck with random values */
        double min = 0.0;
        double max = 1.0;

        for (int n = 0; n < N; n++) {
            center[n] = Math.random() * (max - min) + min;
        }

    }

    public double[] getCenter(){
        return center;
    }

    public void setPosition(double[] pattern){
    }

}

public class MultiGas {

    /* constructor */

    /* parameters:
     *
     * M number of partner network-Gas
     * K number of neurons
     *
     * N size of the input dimension and center vectors
     *
     * */

    double learningRateInit = 0.9;
    double learningRateEnding = 0.1;

    int M, N, K;
    private Neuron[][] gas = new Neuron[M][K];

    public MultiGas(int M, int N, int K) {

        this.M = M;
        this.N = N;
        this.K = K;

        gas = new Neuron[M][K];

        for (int m = 0; m < M; m++) {
            for (int k = 0; k < K; k++) {
                gas[m][k] = new Neuron(N);
            }
        }

    }

    public void applyStimulus(double[] stimulus){

        /* apply stimulus to each neuron using Euclidean distance |Ck - pX| */

        double dist = 0.0;

        int indexPartnerGasWinner = 0;
        int indexNeuronWinner = 0;

        for (int m = 0; m < M; m++) {
            for (int k = 0; k < K; k++) {
                dist = euclideanDistance(gas[m][k].getCenter(), stimulus);
            }
        }

    }

    private static double euclideanDistance(double[] ck, double[] px) {

        /* ToDo: verify that ck and px have the same size */

        double sum = 0.0;

        for(int i=0; i < ck.length; i++) {
            sum = sum + Math.pow(px[i] - ck[i], 2);
        }
        return Math.sqrt(sum);

    }

    public void Move() {
    }

    public void Print() {
    }

}

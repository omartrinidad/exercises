

class Neuron {

    double[] center;

    public void initCenter(int N){
        this.center = new double[N];
    }

    public void setPosition(double[] pattern){
        center = pattern;
    }

}

public class MultiGas {
    /* */
    /* constructor */
    /* parameters:
     * patterns
     * number of input spaces
     * */

    double learningRateInit = 0.9;
    double learningRateEnding = 0.1;

    int M;
    int N;
    int K;

    public MultiGas(int M, int N, int K) {
        /* */
        Neuron[][] gas = new Neuron[M][K];
        double[] components;

        this.M = M;
        this.N = N;
        this.K = K;

        for (int m = 0; m < M; m++) {
            for (int k = 0; k < K; k++) {
                gas[m][k].initCenter(N);
            }
        }
    }

    public void initCenters(double[][] patterns) {

    }

    public void Train(double[][] patterns){

        int I = 1000;
        int J = 2;

        for (int i = 0; i < I; i++){
            for (int j = 0; j < J; j++){
            }
        }

    }

    public void Move() {
    }

    public void Print() {
    }


}

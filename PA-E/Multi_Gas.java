import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.List;
import java.io.*;

/**
 * @author      Shinho Kang, 					2890169,	wis.shinho.kang@gmail.com
 * @author 		Omar Trinidad Gutierrez Mendez,	2850441,	omar.vpa@gmail.com
 * 
 */
public class Multi_Gas {
	/**
	 * Variables
	 */

    private final int M;				// number of gases (partners)
    private final int[] neuronsInGas;	// the number of neurons in each gas(partner)
    private final int N;				// input dimension & center vector dimension
    
    private Gas[] gas;
    private double[][] patterns;	// pattern data
    
    private final double learningRateInit = 0.5;	// learning rate - initial
    private final double learningRateEnd = 0.001;	// learning rate - end
    private final double gaussianSize = 0.5;		// gaussian bell size (for neighborhood function)
    private int tMax = 10000;						// max training number.
    										// for implementing decaying of learning rate 
    										// and gaussian size
    
    /**
     * constructor : initialize variables
     */
    public Multi_Gas(int M, int[] neuronsInGas, int N) {
    	this.M = M;
    	this.neuronsInGas = neuronsInGas;
    	this.N = N;
    	// initilize gas groups
    	gas = new Gas[M];
    	for (int m = 0; m < M; m++) {
    		gas[m] = new Gas(neuronsInGas[m], N);
    	}
    }
    
    /**
     * initialize center vectors of all the neurons of every gas
     */
    private void initCenters() {
    	for (int m = 0; m < M; m++) {
    		gas[m].initCenters();
    	}
    }
    
    /**
     * read patterns from the given file
     *
     * @param  fileName file name which indicate the data file.
     * @return patterns which is read from the file.
     */
    private void readFile(String fileName) {
        try {
            List<String> lines = Files.readAllLines(Paths.get(fileName));
            patterns = new double[lines.size()][N];
            tMax = lines.size();
            
            int l = 0;
            for (String line : lines) {
            	// delimiter: "  " two spaces
                String[] values = line.split("  ");
            	
                for (int n = 0; n < N; n++) {
                    patterns[l][n] = Double.parseDouble(values[n]);
//                    System.out.print( patterns[l][n] );
//                    System.out.print(" ");
                }
//                System.out.print("\n");
            	l++;
			}
        } catch (FileNotFoundException e) {
            System.out.println("The filename you provided it was not found");
            System.exit(0);
        } catch (IOException e) {
			e.printStackTrace();
			System.exit(0);
		}
    }

    /**
     * print out center positions into a file.
     */
    private void printCenters(String printFileName) {
    	try {
			PrintWriter fout = new PrintWriter(new BufferedWriter(
					new FileWriter(printFileName)));
			for (int m = 0; m < M; m++) {
				for (int j = 0; j < gas[m].neurons.length; j++) {
					double[] center = gas[m].neurons[j].getCenter();
					for (int k = 0; k < center.length; k++) {
						fout.printf("%f ", center[k]); 
					}
					fout.printf("\n");
				}
			}
			fout.close();
		} catch (IOException e) {
			e.printStackTrace();
		}
    }

    /**
     * train multi-gas by stimulus
     * 
     * for each stimulus:
     * - compute all the responses
     * - pick winner neuron and gas
     * - generate sorted list respect to the response of the nuerons in the winner gas
     * - move the neurons in the winner gas
     */
    private void train() {
    	for (int t=0; t < tMax; t++) {
    		System.out.printf("Epoch: %d\n", t);
    		int winnerGasIndex = 0;
    		double tmpShortestDist = 0;
    		for (int m=0; m < M; m++) {
    			// calculate all the responses in the gas
    			gas[m].calcResponses(patterns[t]);
    			if (m==0)
    				tmpShortestDist = gas[0].shortestDistance;
    			else {
    				// pick winner gas
    				if (gas[m].shortestDistance < tmpShortestDist) {
    					winnerGasIndex = m;
    					tmpShortestDist = gas[m].shortestDistance; 
    				}
    			}
    			System.out.printf("-%d : %f\n", m, gas[m].shortestDistance);
    		}
    		System.out.printf("Winner Gas is %d\n", winnerGasIndex);
			// generate sorted list respect to the response of the nuerons in the winner gas
			//gas[winnerGasIndex]
    	}
    }
    
    /**
     * Main
     * 
     * - construct Multi Gas - Gases, neurons, centers
     * - initialize centers of the neurons
     * - read given data file
     * - train by stimulus
     * - print out positions of centers
     */
    public static void main(String[] args) {
		String patternFileName = "train_PA-E.dat";
		if(args.length > 0 &&  !args[0].isEmpty()) {
			patternFileName = args[0];
		}
//        System.out.print(patternFileName);	

		// Setting variables        
		int M = 4;	// number of gases (partners)
		int[] neuronsInGas = { 5, 7, 10, 3 };	// the number of neurons in each gas(partner)
		int N = 2;	// input dimension & center vector dimension
		
		// construct Multi Gas - Gases, neurons, centers
		Multi_Gas multiGas = new Multi_Gas(M, neuronsInGas, N);
		// initialize centers of the neurons
		multiGas.initCenters();
		
		// read given data file 
		multiGas.readFile(patternFileName);
		
		// train by stimulus
		multiGas.train();
		
		// print out positions of centers
		multiGas.printCenters("PA-E.net");
//		
      
    }
    
    
    
    
    
    /**
     * Gas class
     *  - include Neurons
     */
    public class Gas {
    	private Neuron[] neurons;
    	private double shortestDistance;	// shortest distance between the neuron in the gas and the stimulus
    	private Neuron[] sortedNeurons;		// neurons which are sorted by distance 
    	/**
    	 * constructor
    	 * 
    	 * @param neuronsInGas determine the number of neurons in this gas
    	 * @param N determine the dimension of center vectors of the neurons
    	 */
    	public Gas(int neuronsInGas, int N) {
    		this.neurons = new Neuron[neuronsInGas];

    		// to initialize each neuron. 
    		for (int i = 0 ; i < neuronsInGas; i++) {
    			neurons[i] = new Neuron(i, N);
    		}
    	}
    	
    	/**
    	 * initilize centers in the gas by randomly generated position
    	 */
    	private void initCenters() {
    	    for (int i = 0; i < neurons.length; i++) {
    	    	int N = neurons[i].center.length;
    	    	double[] rand_position = new double[N];
        		
    	    	/* Initialize Ck with random values */
    	        double min = 0.0;
    	        double max = 1.0;

    	        for (int n = 0; n < N; n++) {
    	        	rand_position[n] = Math.random() * (max - min) + min;
    	        }
    	    	neurons[i].setPosition(rand_position);
    		}
    	}  
    	/**
    	 * calculate all the responses in the gas
    	 * and store shortest distance 
    	 */
    	private void calcResponses(double[] stimulus) {
    		this.shortestDistance = 9999999;
    		double dist = 0;
    		 
    		for (int i=0; i < neurons.length; i++) {
    			dist = neurons[i].calculateDistance(stimulus);
    			if (dist < this.shortestDistance) {
    				this.shortestDistance = dist;
    			}
    		}
    	}
    }
    
    /**
     * Neuron class
     *  - each neuron object includes center vector
     */
    public class Neuron {
    	private final int initialIndex;		// initial index of neuron in the gas
    	private double[] center;			// center vector
        private double distanceToStimulus;	// calculated distance from center to stimulus
        
        /**
         * constructor
         * 
         * @param i initial index of the neuron in the gas
         * @param N dimension of center vector
         */
        public Neuron(int i, int N) {
            this.initialIndex = i;
        	center = new double[N];
        }
        

        private double[] getCenter() {
            return center;
        }
        /**
         * move center to the given position
         */
        private void setPosition(double[] position) {
        	if (center.length != position.length) {
        		//throw new Exception("length error");
        	}
        	center = position;
        }
        /**
         * move center by adding movingFactor
         */
        private void movePosition(double[] movingFactor) {
        	if (center.length != movingFactor.length) {
        		//throw new Exception("length error");
        	}
        	for (int i = 0; i < movingFactor.length; i++) {
        		center[i] += movingFactor[i];
        	}
        }
        
        /**
         * calculate the euclidean distance between center and stimulus
         */
        private double calculateDistance(double[] stimulus) {
        	this.distanceToStimulus = 0;
        	
        	// TODO: implement calculation of distance
        	double sum = 0.0;

            for(int i=0; i < stimulus.length; i++) {
                sum = sum + Math.pow(stimulus[i] - center[i], 2);
            }
            this.distanceToStimulus = Math.sqrt(sum);
            
        	return this.distanceToStimulus;
        }
    }
}


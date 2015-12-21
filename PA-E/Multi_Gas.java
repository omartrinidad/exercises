import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.*;
import java.io.*;

/**
 * @author      Shinho Kang, 					2890169,	wis.shinho.kang@gmail.com
 * @author 		Omar Trinidad Gutierrez Mendez,	2850441,	omar.vpa@gmail.com
 * 
 * Compile : javac Multi_Gas.java
 * execute : java Multi_Gas
 * 
 * This program print out 2 files.
 *  - "PA-E.net" : the final position of the centers 
 *  - "PA-E_patterns.net" : read or generated input patterns
 */
public class Multi_Gas {
	/**
	 * Variables
	 */
    private final int M;				// number of gases (partners)
    private final int[] neuronsInGas;	// the number of neurons in each gas(partner)
    private final int N;				// input dimension & center vector dimension
    
    private Gas[] gas;				// partner gases
    private double[][] patterns;	// pattern data
    
    private final double learningRateInit = 0.8;	// learning rate - initial
    private final double learningRateEnd = 0.01;	// learning rate - end
    private final double gaussianSize = 0.8;		// gaussian bell size (for neighborhood function)
    private int tMax = 2000;						// max training number.
    												// for implementing decaying of learning rate 
    
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
     * generate patterns drawn randomly, 
     * equally distributed from 3 non-overlapping areas  
     */
    private void generatePatterns() {
    	// N = 2
    	patterns = new double[tMax][2];
    	
    	Random rand_area = new Random();
    	Random rand_x = new Random();
    	Random rand_y = new Random();
    	
    	// Initialize center with random values
        double min = 0.0;
        double max = 1.0;
    	
    	// 3 non-overlapping areas: {0.0, 0.33} {0.4, 0.8}, {0.33
    	for (int t=0; t < tMax; t++) {
            int area = rand_area.nextInt(9) % 3;	// area 0~2
            
    		switch (area) {
    		case 0:
    			min = 0.8;
    			max = 1.0;
    			break;
    		case 1:
    			min = 0.3;
    			max = 0.7;
    			break;
    		case 2:
    			min = 0.0;
    			max = 0.25;
    			break;
    		}
    		
    		double x = rand_x.nextDouble() * (max - min) + min;
    		double y = rand_y.nextDouble() * (max - min) + min;
//    		System.out.printf("[%d] %f %f\n", area, x, y);
    		
    		patterns[t][0] = x;
    		patterns[t][1] = y;
    	}

    	printPatterns("PA-E_patterns.net");    	
    	
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

    private void printPatterns(String printFileName) {
    	try {
			PrintWriter fout = new PrintWriter(new BufferedWriter(
					new FileWriter(printFileName)));
			for (int t = 0; t < tMax; t++) {
				for (int n = 0; n < N; n++) {
					fout.printf("%f ", patterns[t][n]); 
				}
				fout.printf("\n");
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
    	// for each stimulus
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
			gas[winnerGasIndex].generateSortedList();
			// learn by stimulus only for neurons of winner gas
			gas[winnerGasIndex].learn(t, learningRate(t), gaussianSize, patterns[t]);
    	}
    }
    /**
     * decaying of learningRate by training epochs
     */
    private double learningRate(int t) {
    	return learningRateInit * Math.pow((learningRateEnd / learningRateInit), (t * 1.0 / tMax));
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

		// Setting variables        
		int M = 4;	// number of gases (partners)
		int[] neuronsInGas = { 7, 5, 6, 7 };	// the number of neurons in each gas(partner)
		int N = 2;	// input dimension & center vector dimension
		
		
		// construct Multi Gas - Gases, neurons, centers
		Multi_Gas multiGas = new Multi_Gas(M, neuronsInGas, N);
		// initialize centers of the neurons
		multiGas.initCenters();

		// read given data file - uncomment this to read patterns from data file 
		//multiGas.readFile(patternFileName);
		
		// to generate random patterns on 3-non overlapping areas
		// uncomment this to generate random pattern data
		multiGas.generatePatterns();

		// train by stimulus
		multiGas.train();

		// print out positions of centers
		multiGas.printCenters("PA-E.net");
    }
    
    
    /**
     * Gas class
     *  - include Neurons
     */
    public class Gas {
    	private Neuron[] neurons;	 
    	private double shortestDistance;	// shortest distance between the neuron in the gas and the stimulus
    	private Pair[] sortedNeuronIndex;  
    	
    	/**
    	 * constructor
    	 * 
    	 * @param neuronsInGas determine the number of neurons in this gas
    	 * @param N determine the dimension of center vectors of the neurons
    	 */
    	public Gas(int neuronsInGas, int N) {
    		this.neurons = new Neuron[neuronsInGas];

    		// to initialize neurons in the gas. 
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
        		Random rand = new Random();
    	    	/* Initialize center with random values */
    	        double min = 0.0;
    	        double max = 1.0;

    	        for (int n = 0; n < N; n++) {
    	        	rand_position[n] = rand.nextDouble() * (max - min) + min;
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
    	
    	/**
    	 * sort neurons respect to the distance
    	 * closest neuron is positioned on 0. 
    	 */
    	private void generateSortedList() {
    		// use Pair class and custom compare method
    		sortedNeuronIndex = new Pair[neurons.length];
    		for (int i=0; i < neurons.length; i++) {
    			sortedNeuronIndex[i] = new Pair(neurons[i].initialIndex, neurons[i].distanceToStimulus);
    		}
    		Arrays.sort(sortedNeuronIndex);
    		
    		// to check
    		for (int i=0; i < neurons.length; i++) {
    			System.out.printf("[%d]%f ", sortedNeuronIndex[i].index, sortedNeuronIndex[i].distance); 
    		}
    	}
    	
    	/**
    	 * learn by stimulus
    	 * 
    	 * @param t training epoch
    	 * @param learningRate decayed learning rate
    	 * @param gaussianSize size for gaussian function 
    	 * @param stimulus selected stimulus
    	 */
    	private void learn(int t, double learningRate, double gaussianSize, double[] stimulus) {
    		System.out.printf("\nstimulus: [%f, %f]\n",  stimulus[0], stimulus[1]);
    		// learn by sorted order
    		for (int i = 0; i < sortedNeuronIndex.length; i++) {
    			// calculate neighborhood function (gaussian function)
    			double h = Math.exp(-Math.pow(i, 2) / (2 * Math.pow(gaussianSize, 2)));
    			
    			int originIndex = sortedNeuronIndex[i].index;	
    			// calculate difference between center and stimulus
    			double[] movingFactor = neurons[originIndex].calculateDiff(stimulus);
    			
    			System.out.printf("[%d] neuron : [%f, %f], dist : %f\n", originIndex, neurons[originIndex].center[0]
    					,neurons[originIndex].center[1], neurons[originIndex].distanceToStimulus);
    			System.out.printf("  move: [%f, %f], rate: %f\n", movingFactor[0],movingFactor[1],learningRate * h); 

    			// move neuron (update)  
    			neurons[originIndex].movePosition(movingFactor, learningRate * h);
    			System.out.printf(" -> moved: [%f, %f]\n", neurons[originIndex].center[0], neurons[originIndex].center[1]);
    		}
    	} 
    	
    	/**
    	 * Pair class
    	 *  include index and distance
    	 *  implement override compareTo method
    	 */
    	public class Pair implements Comparable<Pair> {
    	    public final int index;
    	    public final double distance;

    	    public Pair(int index, double distance) {
    	        this.index = index;
    	        this.distance = distance;
    	    }

    	    @Override
    	    public int compareTo(Pair other) {
    	        //multiplied to -1 as the author need descending sort order
    	    	return 1 * Double.valueOf(this.distance).compareTo(other.distance);
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
        
        /**
         * return center position
         */
        private double[] getCenter() {
            return center;
        }
        /**
         * move center to the given position
         */
        private void setPosition(double[] position) {
        	center = position;
        }
        /**
         * move center by adding movingFactor * rate
         */
        private void movePosition(double[] movingFactor, double rate) {
        	for (int i = 0; i < movingFactor.length; i++) {
        		center[i] += movingFactor[i] * rate;
        	}
        }
        
        /**
         * calculate the euclidean distance between center and stimulus
         * store the distance and return
         */
        private double calculateDistance(double[] stimulus) {
        	this.distanceToStimulus = 0;
        	double sum = 0.0;

            for(int i=0; i < stimulus.length; i++) {
                sum = sum + Math.pow(stimulus[i] - center[i], 2);
            }
            this.distanceToStimulus = Math.sqrt(sum);
            
        	return this.distanceToStimulus;
        }
        /**
         * calculation vector diff between center and stimulus
         */
        private double[] calculateDiff(double[] stimulus) {
        	double[] diff = new double[stimulus.length];
        	for (int i=0; i < stimulus.length; i++) {
        		diff[i] = stimulus[i] - this.center[i];
        	}
        	
        	return diff;
        }
    }
}


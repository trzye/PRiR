import org.apache.log4j.Logger;

import java.util.ArrayList;

/**
 * Created by Michał on 2016-03-16.
 */
public class NormCalculator {

    final Logger LOG = Logger.getLogger(NormCalculator.class);
    private final ArrayList<Thread> threads = new ArrayList<>();
    Matrix C;
    Indexes threadIndexes;
    private int threadNumber;


    public NormCalculator(final Matrix C, final int threadNumber) throws InterruptedException {
        this.C = C;
        C.setNorm(0.0f);
        this.threadNumber = threadNumber;

        final Integer indexes = C.rows();
        this.LOG.debug(String.format("Liczba indeksow: %d", indexes));


        if (threadNumber > indexes) {
            this.LOG.warn(String.format("Wątków[%d] jest więcej niż indeksow[%d] do przetworzenia. Program utworzy tylko tyle watkow ile jest indeksow.", this.threadNumber, indexes));
            this.threadNumber = indexes;
        }

        this.threadIndexes = new Indexes(indexes, this.threadNumber);
        this.LOG.debug(String.format("Podział indeksów: %s", this.threadIndexes));

        this.calculate();
        this.join();
    }

    private void join() throws InterruptedException {
        for (int j = 0; j < this.threadNumber; j++) {
            this.threads.get(j).join();
        }
    }

    private void calculate() {
        for (int j = 0; j < this.threadNumber; j++) {
            calculate(this.threadIndexes.firstIndex(j), this.threadIndexes.lastIndex(j));
        }
    }

    private void calculate(final int index1, final int index2) {
        final CalculateNormThread calculateThread = new CalculateNormThread(this.C, index1, index2);
        calculateThread.start();
        this.threads.add(calculateThread);
        try {
            calculateThread.join();
        } catch (final InterruptedException e) {
            e.printStackTrace();
        }
    }

    public double getResult() {
        return Math.sqrt(this.C.getNorm());
    }
}

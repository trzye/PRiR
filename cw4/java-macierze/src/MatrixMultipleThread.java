import org.apache.log4j.Logger;

import java.util.ArrayList;

/**
 * User: szmurlor
 * Date: 17.03.14
 * Time: 12:13
 */
public class MatrixMultipleThread {

    final Logger LOG = Logger.getLogger(MatrixMultipleThread.class);

    private final Matrix C;
    private final Matrix A;
    private final Matrix B;
    private final ArrayList<Thread> threads = new ArrayList<>();
    private final Indexes threadIndexes;
    private int threadNumber;

    public MatrixMultipleThread(final Matrix A, final Matrix B, final int threadNumber) throws InterruptedException {
        this.A = A;
        this.B = B;
        this.threadNumber = threadNumber;
        this.C = new Matrix(A.rows(), B.cols());

        final Integer indexes = A.rows();
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
        final CalculateThread calculateThread = new CalculateThread(this.A, this.B, this.C, index1, index2);
        calculateThread.start();
        this.threads.add(calculateThread);
    }

    public Matrix getResult() {
        return this.C;
    }
}

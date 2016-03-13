import org.apache.log4j.Logger;

/**
 * Created by Michał on 2016-03-13.
 */
public class CalculateThread extends Thread {

    final Logger LOG = Logger.getLogger(MatrixMultipleThread.class);

    private final Matrix C;
    private final Matrix A;
    private final Matrix B;
    private final int index1;
    private final int index2;

    public CalculateThread(final Matrix A, final Matrix B, final Matrix C, final int index1, final int index2) {
        this.A = A;
        this.B = B;
        this.C = C;
        this.index1 = index1;
        this.index2 = index2;
        this.LOG.debug(String.format("Watek [%s], przygotowany do liczenia indeksow [%d -> %d]", this.getId(), index1, index2));
    }

    @Override
    public void run() {
        for (int r = this.index1; r <= this.index2; r++) {
            for (int c = 0; c < this.B.cols(); c++) {
                float s = 0;
                for (int k = 0; k < this.A.cols(); k++) {
                    s += this.A.get(r, k) * this.B.get(k, c);
                }
                this.C.set(r, c, s);
            }
        }
        this.LOG.debug(String.format("Watek [%s], obliczyl indeksy [%d -> %d]", this.getId(), this.index1, this.index2));
    }
}

import org.apache.log4j.Logger;

/**
 * Created by Michał on 2016-03-16.
 */
public class CalculateNormThread extends Thread {
    final Matrix C;
    final Logger LOG = Logger.getLogger(CalculateNormThread.class);
    int index1, index2;


    public CalculateNormThread(final Matrix C, final int index1, final int index2) {
        this.C = C;
        this.index1 = index1;
        this.index2 = index2;
        this.LOG.debug(String.format("Watek [%s], przygotowany do liczenia indeksow [%d -> %d]", this.getId(), index1, index2));
    }

    @Override
    public void run() {
        float res = 0.0f;

        for (int i = this.index1; i <= this.index2; i++) {
            for (int j = 0; j < this.C.cols(); j++) {
                res += this.C.get(i, j) * this.C.get(i, j);
            }
        }

        synchronized (this.C) {
            this.C.setNorm(this.C.getNorm() + res);
        }

        this.LOG.debug(String.format("Watek [%s], obliczyl indeksy [%d -> %d]", this.getId(), this.index1, this.index2));

        super.run();
    }
}

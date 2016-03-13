/**
 * User: szmurlor
 * Date: 17.03.14
 * Time: 12:13
 */
public class MatrixMultiple {

    private final Matrix C;
    private final Matrix A;
    private final Matrix B;

    public MatrixMultiple(final Matrix A, final Matrix B) {
        this.A = A;
        this.B = B;
        this.C = new Matrix(A.rows(), B.cols());
    }

    public void calculate() {
        for (int r = 0; r < this.A.rows(); r++) {
            for (int c = 0; c < this.B.cols(); c++) {
                float s = 0;
                for (int k = 0; k < this.A.cols(); k++) {
                    s += this.A.get(r, k) * this.B.get(k, c);
                }
                this.C.set(r, c, s);
            }
        }
    }

    public Matrix getResult() {
        return this.C;
    }
}

import java.io.File;
import java.io.FileNotFoundException;
import java.util.Arrays;
import java.util.Locale;
import java.util.Scanner;

/**
 * Created by Michał on 2016-03-13.
 */
public class Matrix {
    private final int ncols;
    private final int nrows;
    private final float[] _data;
    private float norm = 0.0f;

    public Matrix(final int r, final int c) {
        this.ncols = c;
        this.nrows = r;
        this._data = new float[c * r];
    }

    public static Matrix read(final String path) throws FileNotFoundException {
        final File f = new File(path);
        final Scanner scanner = new Scanner(f).useLocale(Locale.ENGLISH);

        final int rows = scanner.nextInt();
        final int cols = scanner.nextInt();
        final Matrix res = new Matrix(rows, cols);

        for (int r = 0; r < res.rows(); r++) {
            for (int c = 0; c < res.cols(); c++) {
                res.set(r, c, scanner.nextFloat());
            }
        }
        return res;
    }

    public float getNorm() {
        return this.norm;
    }

    public void setNorm(final float norm) {
        this.norm = norm;
    }

    public float get(final int r, final int c) {
        return this._data[r * this.ncols + c];
    }

    public void set(final int r, final int c, final float v) {
        this._data[r * this.ncols + c] = v;
    }

    public int rows() {
        return this.nrows;
    }

    public int cols() {
        return this.ncols;
    }

    public String toString() {
        final StringBuilder mString = new StringBuilder("[\n");
        for (int r = 0; r < rows(); r++) {

            for (int c = 0; c < cols(); c++) {
                mString.append(get(r, c));
                mString.append(" ");
            }

            mString.append("\n");
        }
        mString.append("]\n");
        return mString.toString();
    }

    @Override
    public boolean equals(final Object obj) {
        if (obj instanceof Matrix) {
            return Arrays.equals(this._data, ((Matrix) obj)._data);
        }
        return super.equals(obj);
    }
}

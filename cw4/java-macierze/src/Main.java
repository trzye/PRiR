import org.apache.log4j.Logger;

import java.io.FileNotFoundException;

/**
 * Created by Michał on 2016-03-13.
 */
public class Main {

    final Logger LOG = Logger.getLogger(Main.class);

    public static void main(final String[] args) {
        final Main m = new Main();
        try {
            m.run(args);
        } catch (final FileNotFoundException e) {
            e.printStackTrace();
        } catch (final InterruptedException e) {
            e.printStackTrace();
        }
    }

    public void run(final String[] args) throws FileNotFoundException, InterruptedException {
        if (args.length > 2) {
            final Matrix A = Matrix.read(args[0]);

//            Zakomentowane ponieważ, przy dużych macierzach zabraknie pamięci na wypisywanie macierzy.
//            this.LOG.debug("Wczytalem A:\n" + A);

            final Matrix B = Matrix.read(args[1]);

//            Zakomentowane ponieważ, przy dużych macierzach zabraknie pamięci na wypisywanie macierzy.
//            this.LOG.debug("Wczytalem B:\n" + B);

            final Integer threads = Integer.parseInt(args[2]);
            this.LOG.debug(String.format("Liczba watkow: %d", threads));

            long startTime = System.nanoTime();
            this.LOG.info("[Z watkami] Zaczynam obliczenia");
            final MatrixMultipleThread matrixMultipleThread = new MatrixMultipleThread(A, B, threads);
            final long estimatedTimeThread = System.nanoTime() - startTime;
            this.LOG.info(String.format("[Z watkami] Skonczylem obliczenia, czas wykonywania  [%d ns]", estimatedTimeThread));

//            Zakomentowane ponieważ, przy dużych macierzach zabraknie pamięci na wypisywanie wyników.
//            this.LOG.debug("[Z watkami] A*B = \n" + matrixMultipleThread.getResult());

            startTime = System.nanoTime();
            this.LOG.info("[Bez watkow] Zaczynam obliczenia");
            final MatrixMultiple matrixMultiple = new MatrixMultiple(A, B);
            matrixMultiple.calculate();
            final long estimatedTime = System.nanoTime() - startTime;
            this.LOG.info(String.format("[Bez watkow] Skonczylem obliczenia, czas wykonywania [%d ns]", estimatedTime));

//            Zakomentowane ponieważ, przy dużych macierzach zabraknie pamięci na wypisywanie wyników.
//            this.LOG.debug("[Bez watkow] A*B = \n" + matrixMultiple.getResult());

            if (matrixMultipleThread.getResult().equals(matrixMultiple.getResult())) {
                this.LOG.info(String.format("W obu wypadkach macierze wynikowe sa takie same, roznica w czasie [%d ns]", estimatedTime - estimatedTimeThread));
            } else {
                this.LOG.error("Macierze wynikowe NIE sa takie same");
            }

            this.LOG.info("[Z Watkami] Zaczynam obliczenia normy");
            final NormCalculator normCalculator = new NormCalculator(matrixMultiple.getResult(), threads);
            this.LOG.info("[Z Watkami] Skończyłem obliczenia normy, wynik to " + normCalculator.getResult());

            this.LOG.info("[Bez watkow] Zaczynam obliczenia normy");
            this.LOG.info("[Bez watkow] Skończyłem obliczenia normy, wynik to "
                    + NormCalculator.calculateAndGetResultWithoutThreads(matrixMultiple.getResult()));


        } else {
            this.LOG.error("Za mało argumentów wejściowych. Poprawny format to:\n" +
                    "[program] \"Sciezka do macierzy A\" \"Sciezka do macierzy B\" \"Liczba watkow\"");
        }
    }
}

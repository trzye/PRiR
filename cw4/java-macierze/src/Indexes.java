import javafx.util.Pair;

import java.util.HashMap;
import java.util.Map;

/**
 * Created by Michał on 2016-03-13.
 */
public class Indexes {

    private final Map<Integer, Pair<Integer, Integer>> indexesMap = new HashMap<>();
    private final int indexes;
    private final int threads;

    public Indexes(final Integer indexes, final Integer threads) {
        if (threads > indexes) {
            throw new IllegalArgumentException(String.format("Wątków[%d] jest więcej niż indeksow[%d]", threads, indexes));
        }

        this.indexes = indexes;
        this.threads = threads;
        final int interval = indexes / threads;
        for (int j = 0; j < threads - 1; j++) {
            final int index1 = interval * j;
            final int index2 = interval * (j + 1) - 1;
            this.indexesMap.put(j, new Pair<>(index1, index2));
        }
        if (threads > 1) {
            this.indexesMap.put(threads - 1, new Pair<>(this.indexesMap.get(threads - 2).getValue() + 1, indexes - 1));
        } else {
            this.indexesMap.put(0, new Pair<>(0, indexes - 1));
        }
    }

    public int firstIndex(final int nThread) {
        return this.indexesMap.get(nThread).getKey();
    }

    public int lastIndex(final int nThread) {
        return this.indexesMap.get(nThread).getValue();
    }

    @Override
    public String toString() {
        return "Indexes{" +
                "indexesMap=" + this.indexesMap +
                ", indexes=" + this.indexes +
                ", threads=" + this.threads +
                '}';
    }
}

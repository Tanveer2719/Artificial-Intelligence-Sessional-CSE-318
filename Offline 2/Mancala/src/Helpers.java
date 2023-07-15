import java.util.Random;

public class Helpers {

    // private static Random random = new Random();
    // private static int w1 = random.nextInt(41), w2 = random.nextInt(41), w3 = random.nextInt(41), w4 = random.nextInt(41);
    private static int w1 = 10, w2 = 20, w3 = 30, w4 = 40;
    public static int moveEarned = 0, stonesCaptured = 0;

    public static int heuristic1(Board board, int playerId){
        int mStorage, oStorage;
        if(playerId == 1){
            mStorage = board.mancala[0];
            oStorage = board.mancala[7];
        }else{
            mStorage = board.mancala[7];
            oStorage = board.mancala[0];
        }
        return (mStorage - oStorage);
    }

    public static int heuristic2(Board board, int playerId){
        int ms, os;
        if(playerId == 1){
            ms = board.stonesInMySide();
            os = board.stonesInOpponentSide();
        }else{
            ms = board.stonesInOpponentSide();
            os = board.stonesInMySide();
        }

        return (w1*heuristic1(board, playerId) + w2*(ms-os)); 
    }

    public static int heuristic3(Board board, int playerId){ 
        return heuristic2(board, playerId) + w3*moveEarned;
    }

    public static int heuristic4(Board board, int playerId){ 
        return heuristic3(board, playerId) + w4*stonesCaptured;
    }
}

import java.io.FileWriter;
import java.io.IOException;
import java.util.Scanner;

public class Solve {

    private static Scanner scanner = new Scanner(System.in);

    public static Pair<Integer, Integer> adversarialSearch(Board board, int playerId, int depth, int alpha, int beta, boolean minMax, int heuristic){
        
        if(depth == 0 || board.isGameFinished()){
            if(heuristic == 1)
                return new Pair<>(Helpers.heuristic1(board, playerId), -1);
            else if(heuristic == 2)
                return new Pair<>(Helpers.heuristic2(board,playerId), -1);
            else if(heuristic == 3)
                return new Pair<>(Helpers.heuristic3(board, playerId), -1);
            else if(heuristic == 4)
                return new Pair<>(Helpers.heuristic4(board, playerId), -1);
        }
        // minmax true then max(computer)
        if(minMax){
            int maxVal = Integer.MIN_VALUE;
            int playerMove = -1;

            for(int i = 8; i<14; i++){
                if(board.mancala[i] == 0) {
                    continue;
                }

                // Player p = new Player(player.getBoard());
                Board b = new Board(board.mancala);
                
                boolean ret = b.makeMove(i);

                Pair<Integer, Integer> eval = adversarialSearch(b, playerId , depth-1, alpha, beta, ret, heuristic);
                if(eval.first > maxVal){
                    maxVal = eval.first;
                    playerMove = i;
                }

                alpha = Math.max(alpha, maxVal);
                if(alpha >= beta)
                    break;
            }

            return new Pair<>(maxVal,playerMove);
        }
        else{
            int minVal = Integer.MAX_VALUE;
            int playerMove = -1;

            
            for(int i = 1; i<7; i++){
                if(board.mancala[i] == 0){
                    continue;
                } 
                // Player p = new Player(player.getBoard());
                Board b = new Board(board.mancala);
                
                boolean ret = b.makeMove(i);
                
                Pair<Integer, Integer> eval = adversarialSearch(b, playerId, depth-1, alpha, beta, !ret, heuristic);
                        
                if(eval.first < minVal){
                    minVal = eval.first;
                    playerMove = i;
                }

                beta = Math.min(beta, minVal);
                if(alpha >= beta)
                    break;
            }
            return new Pair<>(minVal,playerMove);
        }
        

    }

    public static void aiBotVSaiBot(int h1, int h2, FileWriter writer)throws IOException{
        Board j = new Board();
        

        // System.out.println("The mancala board is: ");
        // j.printBoard();
        
        
        Player player1 = new Player(j); // player 1 is computer
        Player player2 = new Player(j); // player 2 is human

        while(true){
            if(player1.isGameFinished() || player2.isGameFinished())
                break;
            
            while(true){
                if(player1.isGameFinished()){
                    break;
                }
                // System.out.println("aiBot1's turn: ");
                Helpers.moveEarned = 0;
                Helpers.stonesCaptured = 0;
                Pair<Integer, Integer> x = Solve.adversarialSearch(player1.getBoard(), 1,  10, Integer.MIN_VALUE, Integer.MAX_VALUE, true, h1);
                // System.out.println(x.second);
                boolean ret = player1.makeMove(x.second);
                // player1.printBoard();
                
                // try {
                //     Thread.sleep(1000);

                // } catch (InterruptedException e) {
                    
                //     e.printStackTrace();
                // } 
                if(! ret)
                    break;
                
            }
            while(true){
                if(player2.isGameFinished()){
                    break;
                }
                // System.out.println("aiBot2's turn: ");
                Helpers.moveEarned = 0;
                Helpers.stonesCaptured = 0;
                Pair<Integer, Integer> x = Solve.adversarialSearch(player2.getBoard(), 2, 10, Integer.MIN_VALUE, Integer.MAX_VALUE, false, h2);
                // System.out.println(x.second);
                boolean ret = player2.makeMove(x.second);
                // player2.printBoard();
                
                // try {
                //     Thread.sleep(1000);

                // } catch (InterruptedException e) {
                //     e.printStackTrace();
                // }
                if(! ret)
                    break;

            }
        }

        if((player2.getBoard().mancala[7]+player2.getBoard().stonesInOpponentSide()) > (player1.getBoard().mancala[0] + player1.getBoard().stonesInMySide())){
            // System.out.println("AIBOT 2 WINS, SCORE: " + (player2.getBoard().mancala[7]+player2.getBoard().stonesInOpponentSide()));
            writer.write(h1 + "\t"+ h2 + "\t" + "2\t1\n");
        }
        else if((player2.getBoard().mancala[7]+player2.getBoard().stonesInOpponentSide()) < (player1.getBoard().mancala[0] + player1.getBoard().stonesInMySide())){
            // System.out.println("AIBOT 1 WINS, SCORE: " + (player1.getBoard().mancala[0] + player1.getBoard().stonesInMySide()));
            writer.write(h1 + "\t"+ h2 + "\t" + "1\t2\n");
        }
        else{
            // System.out.println("DRAWN");
            writer.write(h1 + "\t"+ h2 + "\t" + "DRAWN\n");

        }

        // System.out.println("GAME ENDED");
        // player1.printBoard();
    }
    
    public static void playerVSaiBot(){
        Board j = new Board();
        
        System.out.println("The mancala board is: ");
        j.printBoard();

        Player player1 = new Player(j); // player 1 is computer
        Player player2 = new Player(j); // player 2 is human

        while(true){
            if(player1.isGameFinished() || player2.isGameFinished())
                break;

            while(true){
                System.out.println("YOUR TURN: ");
                int x = takeInput();
                while(player1.getBoard().mancala[x] == 0){
                    System.out.println("Give valid Move");
                    x = takeInput();
                }
                boolean ret = player1.makeMove(x);

                player1.printBoard();

                if(!ret || player1.isGameFinished()){
                    break;
                }

            }
            while(true){
                if(player2.isGameFinished()){
                    break;
                }
                System.out.println("AIBOT'S TURN: ");
                Helpers.moveEarned = 0;
                Helpers.stonesCaptured = 0;
                Pair<Integer, Integer> x = Solve.adversarialSearch(player2.getBoard(), 1,  15, Integer.MIN_VALUE, Integer.MAX_VALUE, true, 1);
                System.out.println(x.second);
                boolean ret = player2.makeMove(x.second);
                player2.printBoard();
                
                try {
                    Thread.sleep(1000);

                } catch (InterruptedException e) {
                    
                    e.printStackTrace();
                } 

                if(! ret)
                    break;

            }
        }
        if((player2.getBoard().mancala[0]+player2.getBoard().stonesInMySide()) > (player1.getBoard().mancala[7] + player1.getBoard().stonesInOpponentSide())){
            System.out.println("AIBOT WINS, SCORE: " + (player2.getBoard().mancala[0]+player2.getBoard().stonesInMySide()) );
        }
        else if((player2.getBoard().mancala[0]+player2.getBoard().stonesInMySide()) < (player1.getBoard().mancala[7] + player1.getBoard().stonesInOpponentSide())){
            System.out.println("HUMAN WINS, SCORE: " + (player1.getBoard().mancala[7] + player1.getBoard().stonesInOpponentSide()));
        }
        else{
            System.out.println("DRAWN");
        }

        System.out.println("GAME ENDED");
        player1.printBoard();

    } 
    
    private static int takeInput(){
        int x = scanner.nextInt();
        scanner.nextLine();
        while(x < 1 || x > 6){
            System.out.println("Please give a valid move");
            x = scanner.nextInt();
            scanner.nextLine();
        }

        return x;
    }

}

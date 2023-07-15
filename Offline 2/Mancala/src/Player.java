public class Player {

    private Board board;
    
    public Player(){
    }

    public Player(Board board){
        this.board = board;
    }

    public Board getBoard() {
        return board;
    }
    
    public void setBoard(Board board) {
        this.board = board;
    }


    public boolean isGameFinished(){
        return board.isGameFinished();
    }

    public boolean makeMove(int i){
        boolean ret =  board.makeMove(i);
        
        return ret;
    }
        
    public void printBoard() {
        board.printBoard();
    }
    
}

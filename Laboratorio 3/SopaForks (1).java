//-------------------------------------------------------------------------------------------------------------------------------//
//Bibliotecas utilizadas
import java.io.File;
import java.io.FileNotFoundException;
import java.util.Scanner;
import java.util.concurrent.ForkJoinPool;
import java.util.concurrent.RecursiveTask;
//-------------------------------------------------------------------------------------------------------------------------------//
public class SopaForks extends RecursiveTask<int[]> {
    //---------------------------------------------------------------------------------------------------------------------------//
    private static volatile boolean encontrado = false;
    private char[][] grid;
    private String palabra;
    private int fila0, filaf, columna0, columnaf;
    //---------------------------------------------------------------------------------------------------------------------------//
    public SopaForks(char[][] grid, String palabra, int fila0, int filaf, int columna0, int columnaf) {
        this.grid = grid;
        this.palabra = palabra;
        this.fila0 = fila0;
        this.filaf = filaf;
        this.columna0 = columna0;
        this.columnaf = columnaf;
    }
    //---------------------------------------------------------------------------------------------------------------------------//
    //Funcion recursions
    @Override
    protected int[] compute() {
        //------------------------------------------------------------------------------------//
        if (encontrado) {
            return new int[]{-1, -1}; // Devuelve (-1, -1) si la palabra ya fue encontrada
        }
        //------------------------------------------------------------------------------------//
        if (filaf - fila0 < palabra.length() && columnaf - columna0 < palabra.length()) { //Se busca la palabra, retorna la fila y
            return encontrar_p(grid, palabra, fila0, filaf, columna0, columnaf);          //columna en caso de encontrarla, caso 
        } else {                                                                          //contrario se devuelve (-1,-1)
        //------------------------------------------------------------------------------------//    
            int midfila = (fila0 + filaf) / 2;
            int midcolumna = (columna0 + columnaf) / 2;
            SopaForks fork1 = new SopaForks(grid, palabra, fila0, midfila, columna0, midcolumna); //arriba-izquierda
            SopaForks fork2 = new SopaForks(grid, palabra, fila0, midfila, midcolumna + 1, columnaf); //arriba-derecha
            SopaForks fork3 = new SopaForks(grid, palabra, midfila + 1, filaf, columna0, midcolumna); //abajo-izquierda
            SopaForks fork4 = new SopaForks(grid, palabra, midfila + 1, filaf, midcolumna + 1, columnaf); //abajo-derecha
            invokeAll(fork1, fork2, fork3, fork4);
            int[][] resultados = {fork1.join(), fork2.join(), fork3.join(), fork4.join()};
            for (int[] resultado : resultados) {
                if (!(resultado[0] == -1 && resultado[1] == -1)) {
                    return resultado;
                }
            }
            return new int[]{-1, -1};
        }
        //------------------------------------------------------------------------------------//
    }
    //---------------------------------------------------------------------------------------------------------------------------//
    //Funcion para encontrar la palabra en la sopa
    private int[] encontrar_p(char[][] grid, String palabra, int fila0, int filaf, int columna0, int columnaf) {
        for (int i = fila0; i <= filaf; i++) {
            for (int j = columna0; j <= columnaf; j++) {
                if (grid[i][j] == palabra.charAt(0)) {
                    if (horizontal(grid, palabra, i, j) || vertical(grid, palabra, i, j)) {
                        encontrado = true;
                        return new int[]{i+1, j+1};
                    }
                }
            }
        }
        return new int[]{-1, -1};
    }
    //---------------------------------------------------------------------------------------------------------------------------//
    //Funcion booleana que retorna True si es que la palabra se encuentra horizontalmente
    public static boolean horizontal(char[][] grid, String palabra, int fila, int columna) {
        if (columna + palabra.length() > grid.length) {
            return false;
        }
        for (int i = 0; i < palabra.length(); i++) {
            if (grid[fila][columna + i] != palabra.charAt(i)) {
                return false;
            }
        }
        return true;
    }
    //---------------------------------------------------------------------------------------------------------------------------//
    //Funcion booleana que retorna True si es que la palabra se encuentra verticalmente
    public static boolean vertical(char[][] grid, String palabra, int fila, int columna) {
        if (fila + palabra.length() > grid.length) {
            return false;
        }
        for (int i = 0; i < palabra.length(); i++) {
            if (grid[fila + i][columna] != palabra.charAt(i)) {
                return false;
            }
        }
        return true;
    }
    //---------------------------------------------------------------------------------------------------------------------------//
    //Main donde se ejecutan las funciones
    public static void main(String[] args) {
        char[][] grid;
        String palabra;
        try {
            //------------------------------------------------------------------------------------//
            File file = new File("sopa_de_letras.txt");
            Scanner scanner = new Scanner(file);
            int tamano = Integer.parseInt(scanner.nextLine());
            palabra = scanner.nextLine();
            grid = new char[tamano][tamano];                            //Se lee el archivos
            for (int i = 0; i < tamano; i++) {
                String linea = scanner.nextLine().replace(" ", "");
                for (int j = 0; j < tamano; j++) {
                    grid[i][j] = linea.charAt(j);
                }
            }
            scanner.close();
            //------------------------------------------------------------------------------------//
        } catch (FileNotFoundException e) {
            System.out.println("Archivo no encontrado.");
            return;
        }
            //------------------------------------------------------------------------------------//
        try (ForkJoinPool pool = new ForkJoinPool()) {
            //------------------------------------------------------------------------------------//
            SopaForks task = new SopaForks(grid, palabra, 0, grid.length - 1, 0, grid[0].length - 1);
            //------------------------------------------------------------------------------------//
            long iniciot = System.currentTimeMillis();
            int[] pos = pool.invoke(task);                         //Se hace la invocacion y se toma el tiempo
            long finalt = System.currentTimeMillis();
            //------------------------------------------------------------------------------------//
            if (pos[0] == -1 && pos[1] == -1) {
                System.out.println("La palabra no se encontró.");
            } else {
                System.out.println("La palabra comienza en la fila " + pos[0] + " y la columna " + pos[1]);
            }
            System.out.println("Tiempo total de ejecución en ms con Forks: " + (finalt - iniciot));
        }   //------------------------------------------------------------------------------------//
    }
    //---------------------------------------------------------------------------------------------------------------------------//
}
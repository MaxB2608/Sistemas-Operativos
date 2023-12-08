//-------------------------------------------------------------------------------------------------------------------------------//
//Bibliotecas utilizadas
import java.io.*;
import java.util.*;
//-------------------------------------------------------------------------------------------------------------------------------//
public class Sopa {
    //---------------------------------------------------------------------------------------------------------------------------//
    //Funcion para encontrar la palabra en la sopa
    public static int[] encontrar_p(char[][] grid, String palabra) {
        int tamano = grid.length;
        for (int i = 0; i < tamano; i++) {
            for (int j = 0; j < tamano; j++) {
                if (grid[i][j] == palabra.charAt(0)) {
                    if (horizontal(grid, palabra, i, j) || vertical(grid, palabra, i, j)) {
                        return new int[]{i + 1, j + 1};
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
        try {
            //------------------------------------------------------------------------------------//
            File file = new File("sopa_de_letras.txt");
            Scanner scanner = new Scanner(file);
            int tamano = Integer.parseInt(scanner.nextLine());
            String palabra = scanner.nextLine();
            char[][] grid = new char[tamano][tamano];           //Se lee el archivo
            for (int i = 0; i < tamano; i++) {
                String linea = scanner.nextLine();
                for (int j = 0; j < tamano; j++) {
                    grid[i][j] = linea.charAt(j * 2);
                }
            }
            scanner.close();
            //------------------------------------------------------------------------------------//
            long iniciot = System.currentTimeMillis(); //Inicia el reloj
            //---------------------------------------------------------------------------//            
            int[] pos = encontrar_p(grid, palabra);                               //Se busca la palabra y se toma el tiempo
            //---------------------------------------------------------------------------// 
            long finalt = System.currentTimeMillis(); //Termina el reloj
            System.out.println("La palabra comienza en la fila " + pos[0] + " y la columna " + pos[1]);
            System.out.println("Tiempo total de ejecución en ms: " + (finalt - iniciot));
            //------------------------------------------------------------------------------------//
        } catch (FileNotFoundException e) {
            System.out.println("Archivo no encontrado.");
        }
    }
    //---------------------------------------------------------------------------------------------------------------------------//
}
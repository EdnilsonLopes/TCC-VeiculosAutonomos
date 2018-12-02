package com.tcc.carrinho;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.SQLException;

/**
 * Classe que fabrica as conexões
 * 
 * @author Ednilson Brito Lopes
 */
public class ConnectionFactory {
	/**
	 * Conecta ao banco de dados topy.db
	 * 
	 * @return o objeto da conexão
	 */
	public static Connection getConnection() {
		// SQLite connection string

		String url = "jdbc:sqlite:C://Users/Ednilson/Documents/Estudos Arduino/TCC-VeiculosAutonomos/TCC-VeiculosAutonomos/Programas/topy.db";
		Connection conn = null;
		try {
			Class.forName("org.sqlite.JDBC");
			conn = DriverManager.getConnection(url);
		} catch (SQLException e) {
			System.out.println(e.getMessage());
		} catch (ClassNotFoundException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		return conn;
	}

}

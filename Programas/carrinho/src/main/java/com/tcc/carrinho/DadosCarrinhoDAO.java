package com.tcc.carrinho;

import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.List;

public class DadosCarrinhoDAO {

	public static List<DadosCarrinhoVO> dbLoadAll() {
		String sql = "SELECT latitude, longitude, data, hora, distObj FROM dados ORDER BY hora DESC";
		List<DadosCarrinhoVO> dados = new ArrayList<>();

		try (Connection conn = ConnectionFactory.getConnection();
				PreparedStatement ps = conn.prepareStatement(sql);
				ResultSet rs = ps.executeQuery()) {

			// loop through the result set
			while (rs.next()) {
				DadosCarrinhoVO d = new DadosCarrinhoVO();
				d.setLatitude(rs.getString("latitude"));
				d.setLongitude(rs.getString("longitude"));
				d.setData(rs.getString("data"));
				d.setHora(rs.getString("hora"));
				d.setDistObj(rs.getString("distobj"));
				dados.add(d);

			}
		} catch (SQLException e) {
			System.out.println(e.getMessage());
		}
		return dados;
	}

}

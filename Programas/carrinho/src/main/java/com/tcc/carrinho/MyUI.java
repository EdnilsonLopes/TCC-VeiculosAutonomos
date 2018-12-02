package com.tcc.carrinho;

import java.util.List;

import javax.servlet.annotation.WebServlet;

import com.vaadin.annotations.Theme;
import com.vaadin.annotations.VaadinServletConfiguration;
import com.vaadin.data.provider.ListDataProvider;
import com.vaadin.navigator.Navigator;
import com.vaadin.server.VaadinRequest;
import com.vaadin.server.VaadinServlet;
import com.vaadin.ui.Alignment;
import com.vaadin.ui.Button;
import com.vaadin.ui.Grid;
import com.vaadin.ui.Grid.SelectionMode;
import com.vaadin.ui.Label;
import com.vaadin.ui.TextField;
import com.vaadin.ui.UI;
import com.vaadin.ui.VerticalLayout;

/**
 * This UI is the application entry point. A UI may either represent a browser
 * window (or tab) or some part of an HTML page where a Vaadin application is
 * embedded.
 * <p>
 * The UI is initialized using {@link #init(VaadinRequest)}. This method is
 * intended to be overridden to add component to the user interface and
 * initialize non-component functionality.
 */
@Theme("mytheme")
public class MyUI extends UI {

	/**
	 * 
	 */
	private static final long serialVersionUID = -255875709824365291L;

	@Override
	protected void init(VaadinRequest vaadinRequest) {
		final VerticalLayout layout = new VerticalLayout();
		List<DadosCarrinhoVO> dados = DadosCarrinhoDAO.dbLoadAll();
		DadosCarrinhoVO d = new DadosCarrinhoVO();
		ListDataProvider<DadosCarrinhoVO> dadosProv = new ListDataProvider<>(dados);
		Grid<DadosCarrinhoVO> grid = new Grid<>();

		grid.setSizeFull();
		grid.setDataProvider(dadosProv);
		Button button = new Button("Click to Refresh");
		button.addClickListener(e -> {
			//if (e != null)
				//UI.getCurrent().getNavigator().navigateTo("");
		});

		// Set the selection mode
		grid.setSelectionMode(SelectionMode.SINGLE);
		grid.addColumn(DadosCarrinhoVO::getLatitude).setCaption("Latitude");
		grid.addColumn(DadosCarrinhoVO::getLongitude).setCaption("Longitude");
		grid.addColumn(DadosCarrinhoVO::getData).setCaption("Data");
		grid.addColumn(DadosCarrinhoVO::getHora).setCaption("Hora");
		grid.addColumn(DadosCarrinhoVO::getDistObj).setCaption("Distancia");

//        GoogleMap googleMap = new GoogleMap(null, "english");
//        googleMap.setSizeFull();
//        googleMap.addMarker("DRAGGABLE: Paavo Nurmi Stadion", new LatLon(
//                60.442423, 22.26044), true, "VAADIN/1377279006_stadium.png");
////        kakolaMarker = googleMap.addMarker("DRAGGABLE: Kakolan vankila",
////                new LatLon(60.44291, 22.242415), true, null);
//        googleMap.addMarker("NOT DRAGGABLE: Iso-Heikkilä", new LatLon(
//                60.450403, 22.230399), false, null);
//        googleMap.setMinZoom(4);
//        googleMap.setMaxZoom(16);

		layout.addComponents(button, grid);
		layout.setComponentAlignment(button, Alignment.MIDDLE_CENTER);

		setContent(layout);
	}

	@WebServlet(urlPatterns = "/*", name = "MyUIServlet", asyncSupported = true)
	@VaadinServletConfiguration(ui = MyUI.class, productionMode = false)
	public static class MyUIServlet extends VaadinServlet {
	}
}

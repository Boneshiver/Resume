package Utils;

import org.openqa.selenium.WebDriver;
import org.openqa.selenium.WebElement;

import java.util.ArrayList;
import java.util.List;

public class Drivers {
    public WebDriver driver;
    public int lastYLocation = 0;
    public int clicks;
    public int actualClicks = 0;

    public void closeTabs() throws InterruptedException {
        List<String> tabs = new ArrayList<String>(driver.getWindowHandles());
        for (String tab : tabs) {
            Thread.sleep(500);
            driver.switchTo().window(tab).close();
            actualClicks++;
        }
        actualClicks--;
        System.out.println("Clicked " + actualClicks + "/" + clicks + " links!");
    }
}

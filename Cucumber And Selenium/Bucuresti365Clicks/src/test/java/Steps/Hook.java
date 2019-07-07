package Steps;

import Utils.Drivers;

import cucumber.api.java.After;
import cucumber.api.java.Before;
import org.openqa.selenium.chrome.ChromeDriver;
import org.openqa.selenium.firefox.FirefoxDriver;


public class Hook {
    private Drivers base;

    public Hook(Drivers base) {
        this.base = base;
    }

    @Before
    public void InitializeTest(){
        System.out.println("Setting the Web Driver");
//        System.setProperty("webdriver.gecko.driver",String.format("%s\\firefoxDriver\\geckodriver.exe",System.getProperty("user.dir")));//FIREFOX
//        base.driver = new FirefoxDriver();
        System.setProperty("webdriver.chrome.driver",String.format("%s\\chromeDriver\\chromedriver.exe",System.getProperty("user.dir")));//CHROME
        base.driver = new ChromeDriver();
    }
    @After
    public void TeardownTest() throws InterruptedException {
        System.out.println("Closing the Web Driver");
        base.closeTabs();
    }

}

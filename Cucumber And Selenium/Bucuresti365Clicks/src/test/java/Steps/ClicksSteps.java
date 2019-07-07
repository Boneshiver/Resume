package Steps;

import Utils.Drivers;
import cucumber.api.java.en.And;
import cucumber.api.java.en.Given;
import cucumber.api.java.en.Then;
import org.openqa.selenium.By;
import org.openqa.selenium.JavascriptExecutor;
import org.openqa.selenium.Keys;
import org.openqa.selenium.WebElement;
import org.openqa.selenium.interactions.Actions;

import java.util.ArrayList;
import java.util.List;

public class ClicksSteps {
    private Drivers base;

    public ClicksSteps(Drivers base) {
        this.base = base;
    }


    @And("^Selects the subcategory ([^\"]*)$")
    public void selectsTheSubcategorySubcategory(String subcategory) {
        base.driver.findElement(By.linkText(subcategory)).click();
        System.out.println("Selected \"" + subcategory + "\" subcategory");
    }


    @Then("^Clicks on link buttons$")
    public void clicksOnLinkButtons() throws InterruptedException {
        Actions ctrlClick = new Actions(base.driver);
        List<WebElement> presentButtons = base.driver.findElements(By.className("visit-site"));
        base.clicks = presentButtons.size();
        if (base.clicks > 0) {
            JavascriptExecutor jsx = (JavascriptExecutor) base.driver;
            int stop = 0;
            for (WebElement we : presentButtons) {
                jsx.executeScript(String.format("window.scrollBy(0,%s)", we.getLocation().y - base.lastYLocation), "");
                base.lastYLocation = we.getLocation().y;
                ctrlClick.keyDown(Keys.LEFT_CONTROL).click(we).build().perform();
                System.out.println("Tried to click " + we.getAttribute("href"));
                stop++;
                if (stop == 5) {
                    Thread.sleep(500);
                    List<String> tabs = new ArrayList<String>(base.driver.getWindowHandles());
                    for (int i = 1; i < tabs.size(); i++) {
                        base.driver.switchTo().window(tabs.get(i));
                        Thread.sleep(200);
                    }
                    for (int i = 1; i < tabs.size(); i++) {
                        Thread.sleep(500);
                        base.driver.switchTo().window(tabs.get(i)).close();
                        base.actualClicks++;
                    }
                    base.driver.switchTo().window(tabs.get(0));
                    stop = 0;
                }

            }
        }
        Thread.sleep(500);
    }

    @Given("^Navigate to Localuri Bucuresti$")
    public void navigateToLocaluriBucuresti() {
        base.driver.navigate().to("https://www.bucuresti365.ro/firme/localuri-bucuresti");
        System.out.println("Navigating to:https://www.bucuresti365.ro/firme/localuri-bucuresti");
    }

    @Given("^Navigate to Copii Bucuresti$")
    public void navigateToCopiiBucuresti() {
        base.driver.navigate().to("https://www.bucuresti365.ro/firme/copii-bucuresti");
        System.out.println("Navigating to:https://www.bucuresti365.ro/firme/copii-bucuresti");
    }

    @Given("^Navigate to Casa si Constructii Bucuresti$")
    public void navigateToCasaSiConstructiiBucuresti() {
        base.driver.navigate().to("https://www.bucuresti365.ro/firme/casa-si-constructii-bucuresti");
        System.out.println("Navigating to:https://www.bucuresti365.ro/firme/casa-si-constructii-bucuresti");
    }

    @Given("^Navigate to COMERT Bucuresti$")
    public void navigateToCOMERTBucuresti() {
        base.driver.navigate().to("https://www.bucuresti365.ro/firme/comert-bucuresti");
        System.out.println("Navigating to:https://www.bucuresti365.ro/firme/comert-bucuresti");
    }

    @Given("^Navigate to Electro-IT Bucuresti$")
    public void navigateToElectroITBucuresti() {
        base.driver.navigate().to("https://www.bucuresti365.ro/firme/electro-it-bucuresti");
        System.out.println("Navigating to:https://www.bucuresti365.ro/firme/electro-it-bucuresti");
    }

    @Given("^Navigate to Frumusete si Cosmetica Bucuresti$")
    public void navigateToFrumuseteSiCosmeticaBucuresti() {
        base.driver.navigate().to("https://www.bucuresti365.ro/firme/fumusete-si-cosmetica-bucuresti");
        System.out.println("Navigating to:https://www.bucuresti365.ro/firme/fumusete-si-cosmetica-bucuresti");
    }

    @Given("^Navigate to Sanatate Bucuresti$")
    public void navigateToSanatateBucuresti() {
        base.driver.navigate().to("https://www.bucuresti365.ro/firme/sanatate-bucuresti");
        System.out.println("Navigating to:https://www.bucuresti365.ro/firme/sanatate-bucuresti");
    }

    @Given("^Navigate to Scoli si Cursuri Bucuresti$")
    public void navigateToScoliSiCursuriBucuresti() {
        base.driver.navigate().to("https://www.bucuresti365.ro/firme/scoli-si-cursuri-bucuresti");
        System.out.println("Navigating to:https://www.bucuresti365.ro/firme/scoli-si-cursuri-bucuresti");
    }

    @Given("^Navigate to Sport si Divertisment Bucuresti$")
    public void navigateToSportSiDivertismentBucuresti() {
        base.driver.navigate().to("https://www.bucuresti365.ro/firme/sport-si-divertisment-bucuresti");
        System.out.println("Navigating to:https://www.bucuresti365.ro/firme/sport-si-divertisment-bucuresti");
    }

    @Given("^Navigate to Serivicii Bucuresti$")
    public void navigateToSeriviciiBucuresti() {
        base.driver.navigate().to("https://www.bucuresti365.ro/firme/servicii-bucuresti");
        System.out.println("Navigating to:https://www.bucuresti365.ro/firme/servicii-bucuresti");
    }

    @Given("^Navigate to Turism Bucuresti$")
    public void navigateToTurismBucuresti() {
        base.driver.navigate().to("https://www.bucuresti365.ro/firme/turism-bucuresti");
        System.out.println("Navigating to:https://www.bucuresti365.ro/firme/turism-bucuresti");
    }

    @Given("^Navigate to Vehicule Bucuresti$")
    public void navigateToVehiculeBucuresti() {
        base.driver.navigate().to("https://www.bucuresti365.ro/firme/vehicule-bucuresti");
        System.out.println("Navigating to:https://www.bucuresti365.ro/firme/vehicule-bucuresti");
    }
}

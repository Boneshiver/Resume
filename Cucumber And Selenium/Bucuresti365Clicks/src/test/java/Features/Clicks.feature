Feature: Click link to sites on Bucuresti365 website

  @Localuri
  Scenario Outline: Customer clicks on Localuri
    Given Navigate to Localuri Bucuresti
    And Selects the subcategory <LOCALURI>
    Then Clicks on link buttons
    Examples:
      | LOCALURI                |
      | Autoservire             |
      | Berarii                 |
      | Brutarii                |
      | Cluburi                 |
      | Catering                |
      | Cafenele                |
      | Ceainarii               |
      | Cofetarii               |
      | Ciocolaterii            |
      | Cafenele Bistro         |
      | Cluburi Biliard         |
      | Distributie Bauturi     |
      | Distributie Cafea       |
      | Mancare la Domiciliu    |
      | Pizzerii                |
      | Patiserii               |
      | Restaurante             |
      | Restaurante Nunti       |
      | Restaurante Libaneze    |
      | Restaurante Chinezesti  |
      | Restaurante Iraniene    |
      | Restaurante Raw Vegan   |
      | Restaurante Frantuzesti |
      | Shaormerii              |

  @Copii
  Scenario Outline: Customer clicks on Copii
    Given Navigate to Copii Bucuresti
    And Selects the subcategory <COPII>
    Then Clicks on link buttons
    Examples:
      | COPII                 |
      | Accesorii Petrecere   |
      | After school          |
      | Animatori Copii       |
      | Cursuri Copii         |
      | Gradinite Particulare |
      | Haine Copii           |
      | Jucarii Copii         |
      | Locuri de Joaca       |
      | Petreceri Copii       |
      | Rechizite Scolare     |
      | Scoli de Vara         |
      | Tabere si excursii    |
      | Teatru pentru copii   |

  @CasaSiConstructii
  Scenario Outline: Customer clicks on Casa si Constructii
    Given Navigate to Casa si Constructii Bucuresti
    And Selects the subcategory <CASA SI CONSTRUCTII>
    Then Clicks on link buttons
    Examples:
      | CASA SI CONSTRUCTII            |
      | Accesorii ridicare si ancor... |
      | Amenajari Curti si Gradini     |
      | Amenajari Interioare           |
      | Cadastru si Intabulare         |
      | Climatizare / Ventilatie       |
      | Copertine, pergole             |
      | Corpuri de Iluminat            |
      | Firme de Arhitectura           |
      | Firme de Constructii           |
      | Foraje Puturi                  |
      | Garduri si Porti               |
      | Gresie si Faianta              |
      | Hidroizolatii                  |
      | Incalzire                      |
      | Inchideri Terase               |
      | Lenjerie de Pat                |
      | Lifturi-Ascensoare             |
      | Materiale de Constructii       |
      | Magazine de Mobila             |
      | Parchet / Laminat              |
      | Prelate                        |
      | Perdele, jaluzele, draperii    |
      | Proiecte Case                  |
      | Sanitare                       |
      | Saltele si paturi              |
      | Tapiterii                      |
      | Tamplarie Lemn                 |
      | Tamplarie PVC                  |
      | Verificare Gaze                |

  @Comert
  Scenario Outline: Customer clicks on Comert
    Given Navigate to COMERT Bucuresti
    And Selects the subcategory <COMERT>
    Then Clicks on link buttons
    Examples:
      | COMERT                       |
      | Anticariat                   |
      | Accesorii Bijuterii Handmade |
      | Aparate anti-daunatori       |
      | Articole personalizate       |
      | Aparatura Medicala           |
      | Articole pescuit             |
      | Birotica si Papetarie        |
      | Ceasuri                      |
      | Decoratiuni Interioare       |
      | Echipamente Horeca           |
      | Filtre Apa                   |
      | Genti, posete, serviete      |
      | Haine                        |
      | Incaltaminte                 |
      | Pet Shop                     |
      | Masini de numarat bani       |
      | Mase Plastice                |
      | Produse Apicole              |
      | Produse Artizanat            |
      | Preparate Carne              |
      | Scule si Unelte              |
      | Site-uri Reduceri            |
      | Supermarketuri               |
      | Tigari Electronice           |

  @ElectroIT
  Scenario Outline: Customer clicks on Electro-IT
    Given Navigate to Electro-IT Bucuresti
    And Selects the subcategory <ELECTRO-IT>
    Then Clicks on link buttons
    Examples:
      | ELECTRO-IT                     |
      | Aparate de Masura si Control   |
      | Electrocasnice                 |
      | Magazine Telefoane Mobile      |
      | Reparatii Aer Conditionat      |
      | Reparatii Frigidere            |
      | Reparatii Masini de Spalat     |
      | Reparatii Televizoare          |
      | Service Calculatoare si Lap... |
      | Service Electrocasnice         |
      | Service Imprimante si Copia... |
      | Servicii IT                    |
      | Service Telefoane Mobile       |
      | Sonorizare, Videoproiectoare   |

  @FrumuseteSiCosmetica
  Scenario Outline: Customer clicks on Frumusete si Cosmetica
    Given Navigate to Frumusete si Cosmetica Bucuresti
    And Selects the subcategory <FRUMUSETE SI COSMETICA>
    Then Clicks on link buttons
    Examples:
      | FRUMUSETE SI COSMETICA         |
      | Centre SPA                     |
      | Cosmetica Canina               |
      | Cursuri de Coafura             |
      | Criolipoliza                   |
      | Dermato-Estetica               |
      | Epilare Definitiva             |
      | Extensii Par                   |
      | Masaje                         |
      | Parfumerii                     |
      | Produse Cosmetice Profesionale |
      | Peruci                         |
      | Remodelare Corporala           |
      | Saloane Coafura                |
      | Saloane de Infrumusetare       |
      | Tatuaje si Piercing            |

  @Sanatate
  Scenario Outline: Customer clicks on Sanatate
    Given Navigate to Sanatate Bucuresti
    And Selects the subcategory <SANATATE>
    Then Clicks on link buttons
    Examples:
      | SANATATE                       |
      | Acupunctura                    |
      | Bioenergie                     |
      | Cabinete Alergologie           |
      | Cabinete Cardiologie           |
      | Cabinete Dermatologice         |
      | Cabinete Fizioterapie          |
      | Cabinete Ginecologice          |
      | Cabinete Neuropsihiatrie       |
      | Cabinete ORL                   |
      | Cabinete Oftalmologice         |
      | Cabinete Pediatrie             |
      | Cabinet Psihiatrie             |
      | Cabinete Psihologice           |
      | Cabinete Stomatologice         |
      | Cabinete Veterinare            |
      | Cabinete Urologie              |
      | Camine de Batrani              |
      | Chirurgie Plastica si Estetica |
      | Clinici Medicale               |
      | Consumabile Medicale           |
      | Estetica Dentara               |
      | Farmacii                       |
      | Farmacii Veterinare            |
      | Fise Medicale                  |
      | Materiale Dentare              |
      | Medicina integrativa           |
      | Medicina de Familie            |
      | Medicina Muncii                |
      | Nutritionisti                  |
      | Produse Naturiste              |
      | Radiologie Dentara             |
      | Salina                         |
      | Terapii Alternative            |
      | Tehnica Dentara                |

  @ScoliSiCursuri
  Scenario Outline: Customer clicks on Scoli si Cursuri
    Given Navigate to Scoli si Cursuri Bucuresti
    And Selects the subcategory <SCOLI SI CURSURI>
    Then Clicks on link buttons
    Examples:
      | SCOLI SI CURSURI               |
      | Cursuri de Arta                |
      | Cursuri de Calificare          |
      | Cursuri Contabilitate          |
      | Cursuri de Dans                |
      | Cursuri Design Vestimentar     |
      | Curs Ghid Turistic             |
      | Cursuri Inot                   |
      | Cursuri IT                     |
      | Cursuri Limbi Straine          |
      | Cursuri Logopedie              |
      | Cursuri Tehnician Veterinar... |
      | Cursuri Unghii False           |
      | Scoli de Soferi                |
      | Scoli Particulare              |

  @SportSiDivertisment
  Scenario Outline: Customer clicks on Sport si Divertisment
    Given Navigate to Sport si Divertisment Bucuresti
    And Selects the subcategory <SPORT SI DIVERTISMENT>
    Then Clicks on link buttons
    Examples:
      | SPORT SI DIVERTISMENT          |
      | Balet                          |
      | Biciclete                      |
      | Cluburi Arte Martiale          |
      | Cursuri Tenis                  |
      | Echipamente si Articole Spo... |
      | Internet Cafe                  |
      | Kangoo Jumps                   |
      | Pilates                        |
      | Pariuri Sportive               |
      | Sali Aerobic                   |
      | Sali Fitness                   |
      | Stranduri-Piscine              |
      | Terenuri de Fotbal             |
      | Team building                  |
      | Yoga                           |

  @Servicii
  Scenario Outline: Customer clicks on Servicii
    Given Navigate to Serivicii Bucuresti
    And Selects the subcategory <SERVICII>
    Then Clicks on link buttons
    Examples:
      | SERVICII                       |
      | Agentii de Publicitate         |
      | Astrologie                     |
      | Arhivare Documente             |
      | Agentii Casting                |
      | Agentii Modeling               |
      | Agentii Imobiliare             |
      | Administrare Imobile           |
      | Babysitting, bone, menajere    |
      | Birou Mediator                 |
      | Cabinete de Avocatura          |
      | Case de Amanet                 |
      | Case de Schimb Valutar         |
      | Colectare Deseuri              |
      | Croitorie                      |
      | Curatatorie Haine              |
      | Detectivi Particulari          |
      | Deratizare, dezinsectie, de... |
      | Dresaj Caini                   |
      | Deblocari Usi                  |
      | Electricieni                   |
      | Firme de Curatenie             |
      | Firme Curierat                 |
      | Firme Contabilitate            |
      | Firme Recuperari Creante       |
      | Formatii muzicale              |
      | Florarii                       |
      | Instalator                     |
      | Inchirieri depozite si birouri |
      | Inchirieri limuzine            |
      | Mutari Mobila                  |
      | Notari Publici                 |
      | Organizari Evenimente          |
      | Paza si Protectie              |
      | Productie Publicitara          |
      | Protectia Muncii               |
      | Reciclare                      |
      | Rent a Car                     |
      | Servicii de Ecologizare        |
      | Servicii foto-video            |
      | Softuri                        |
      | Spalatorii Covoare             |
      | Servicii Funerare              |
      | Taxi                           |
      | Transport Animale              |
      | Transport Marfa                |
      | Tipografii                     |
      | Traducatori autorizati         |
      | Vanzari Bilete                 |
      | Vidanjare                      |

  @Turism
  Scenario Outline: Customer clicks on Turism
    Given Navigate to Turism Bucuresti
    And Selects the subcategory <TURISM>
    Then Clicks on link buttons
    Examples:
      | TURISM            |
      | Agentii de Turism |
      | Camping           |
      | Hosteluri         |
      | Hoteluri          |
      | Pensiuni          |

  @Vehicule
  Scenario Outline: Customer clicks on Vehicule
    Given Navigate to Vehicule Bucuresti
    And Selects the subcategory <VEHICULE>
    Then Clicks on link buttons
    Examples:
      | VEHICULE                      |
      | Anvelope Auto                 |
      | Dezmembrari Auto              |
      | Echipamente service auto      |
      | Economie combustibil          |
      | Geometrie Roti                |
      | Hotel Anvelope                |
      | Folii Auto                    |
      | Instalatii Gpl                |
      | Parbrize Auto                 |
      | Piese Auto                    |
      | Piese Moto                    |
      | Remorci                       |
      | Service Auto                  |
      | Spalatorii Auto               |
      | Statii Itp                    |
      | Spalatorie Auto Self Service  |
      | Tinichigerie, Vopsitorie Auto |
      | Tractari Auto                 |
      | Tuning Auto                   |
      | Vulcanizare                   |
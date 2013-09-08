#include "ModelCountry.h"

ModelCountry::ModelCountry(QObject *parent) :
    QStringListModel(parent)
{
    QStringList listCountries;
    listCountries << trUtf8("Afganistan")
                  << trUtf8("Albania")
                  << trUtf8("Algieria")
                  << trUtf8("Andora")
                  << trUtf8("Angola")
                  << trUtf8("Antigua i Barbuda")
                  << trUtf8("Arabia Saudyjska")
                  << trUtf8("Argentyna")
                  << trUtf8("Armenia")
                  << trUtf8("Australia")
                  << trUtf8("Austria")
                  << trUtf8("Azerbejdżan")
                  << trUtf8("Bahamy")
                  << trUtf8("Bahrajn")
                  << trUtf8("Bangladesz")
                  << trUtf8("Barbados")
                  << trUtf8("Belgia")
                  << trUtf8("Belize")
                  << trUtf8("Benin")
                  << trUtf8("Bhutan")
                  << trUtf8("Białoruś")
                  << trUtf8("Boliwia")
                  << trUtf8("Bośnia i Hercegowina")
                  << trUtf8("Botswana")
                  << trUtf8("Brazylia")
                  << trUtf8("Brunei")
                  << trUtf8("Bułgaria")
                  << trUtf8("Burkina Faso")
                  << trUtf8("Burundi")
                  << trUtf8("Chile")
                  << trUtf8("Chiny")
                  << trUtf8("Chorwacja")
                  << trUtf8("Cypr")
                  << trUtf8("Czad")
                  << trUtf8("Czarnogóra")
                  << trUtf8("Czechy")
                  << trUtf8("Dania")
                  << trUtf8("Demokratyczna Republika Konga")
                  << trUtf8("Dominika")
                  << trUtf8("Dominikana")
                  << trUtf8("Dżibuti")
                  << trUtf8("Egipt")
                  << trUtf8("Ekwador")
                  << trUtf8("Erytrea")
                  << trUtf8("Estonia")
                  << trUtf8("Etiopia")
                  << trUtf8("Fidżi")
                  << trUtf8("Filipiny")
                  << trUtf8("Finlandia")
                  << trUtf8("Francja")
                  << trUtf8("Gabon")
                  << trUtf8("Gambia")
                  << trUtf8("Ghana")
                  << trUtf8("Grecja")
                  << trUtf8("Grenada")
                  << trUtf8("Gruzja")
                  << trUtf8("Gujana")
                  << trUtf8("Gwatemala")
                  << trUtf8("Gwinea Bissau")
                  << trUtf8("Gwinea Równikowa")
                  << trUtf8("Gwinea")
                  << trUtf8("Haiti")
                  << trUtf8("Hiszpania")
                  << trUtf8("Holandia")
                  << trUtf8("Honduras")
                  << trUtf8("Indie")
                  << trUtf8("Indonezja")
                  << trUtf8("Irak")
                  << trUtf8("Iran")
                  << trUtf8("Irlandia")
                  << trUtf8("Islandia")
                  << trUtf8("Izrael")
                  << trUtf8("Jamajka")
                  << trUtf8("Japonia")
                  << trUtf8("Jemen")
                  << trUtf8("Jordania")
                  << trUtf8("Kambodża")
                  << trUtf8("Kamerun")
                  << trUtf8("Kanada")
                  << trUtf8("Katar")
                  << trUtf8("Kazachstan")
                  << trUtf8("Kenia")
                  << trUtf8("Kirgistan")
                  << trUtf8("Kiribati")
                  << trUtf8("Kolumbia")
                  << trUtf8("Komory")
                  << trUtf8("Kongo")
                  << trUtf8("Korea Południowa")
                  << trUtf8("Korea Północna")
                  << trUtf8("Kostaryka")
                  << trUtf8("Kuba")
                  << trUtf8("Kuwejt")
                  << trUtf8("Laos")
                  << trUtf8("Lesotho")
                  << trUtf8("Liban")
                  << trUtf8("Liberia")
                  << trUtf8("Libia")
                  << trUtf8("Liechtenstein")
                  << trUtf8("Litwa")
                  << trUtf8("Luksemburg")
                  << trUtf8("Łotwa")
                  << trUtf8("Macedonia")
                  << trUtf8("Madagaskar")
                  << trUtf8("Malawi")
                  << trUtf8("Malediwy")
                  << trUtf8("Malezja")
                  << trUtf8("Mali")
                  << trUtf8("Malta")
                  << trUtf8("Maroko")
                  << trUtf8("Mauretania")
                  << trUtf8("Mauritius")
                  << trUtf8("Meksyk")
                  << trUtf8("Mikronezja")
                  << trUtf8("Mjanma")
                  << trUtf8("Mołdawia")
                  << trUtf8("Monako")
                  << trUtf8("Mongolia")
                  << trUtf8("Mozambik")
                  << trUtf8("Namibia")
                  << trUtf8("Nauru")
                  << trUtf8("Nepal")
                  << trUtf8("Niemcy")
                  << trUtf8("Niger")
                  << trUtf8("Nigeria")
                  << trUtf8("Nikaragua")
                  << trUtf8("Norwegia")
                  << trUtf8("Nowa Zelandia")
                  << trUtf8("Oman")
                  << trUtf8("Pakistan")
                  << trUtf8("Palau")
                  << trUtf8("Panama")
                  << trUtf8("Papua-Nowa Gwinea")
                  << trUtf8("Paragwaj")
                  << trUtf8("Peru")
                  << trUtf8("Polska")
                  << trUtf8("Portoryko")
                  << trUtf8("Portugalia")
                  << trUtf8("Republika Południowej Afryki")
                  << trUtf8("Republika Środkowoafrykańska")
                  << trUtf8("Republika Zielonego Przylądka")
                  << trUtf8("Rosja")
                  << trUtf8("Rumunia")
                  << trUtf8("Rwanda")
                  << trUtf8("Sahara Zachodnia")
                  << trUtf8("Saint Kitts i Nevis")
                  << trUtf8("Saint Lucia")
                  << trUtf8("Saint Vincent i Grenadyny")
                  << trUtf8("Salwador")
                  << trUtf8("Samoa")
                  << trUtf8("San Marino")
                  << trUtf8("Senegal")
                  << trUtf8("Serbia")
                  << trUtf8("Seszele")
                  << trUtf8("Sierra Leone")
                  << trUtf8("Singapur")
                  << trUtf8("Słowacja")
                  << trUtf8("Słowenia")
                  << trUtf8("Somalia")
                  << trUtf8("Sri Lanka")
                  << trUtf8("Stany Zjednoczone")
                  << trUtf8("Suazi")
                  << trUtf8("Sudan Południowy")
                  << trUtf8("Sudan")
                  << trUtf8("Surinam")
                  << trUtf8("Syria")
                  << trUtf8("Szwajcaria")
                  << trUtf8("Szwecja")
                  << trUtf8("Tadżykistan")
                  << trUtf8("Tajlandia")
                  << trUtf8("Tanzania")
                  << trUtf8("Timor Wschodni")
                  << trUtf8("Togo")
                  << trUtf8("Tonga")
                  << trUtf8("Trynidad i Tobago")
                  << trUtf8("Tunezja")
                  << trUtf8("Turcja")
                  << trUtf8("Turkmenistan")
                  << trUtf8("Tuvalu")
                  << trUtf8("Uganda")
                  << trUtf8("Ukraina")
                  << trUtf8("Urugwaj")
                  << trUtf8("Uzbekistan")
                  << trUtf8("Vanuatu")
                  << trUtf8("Watykan")
                  << trUtf8("Wenezuela")
                  << trUtf8("Węgry")
                  << trUtf8("Wielka Brytania")
                  << trUtf8("Wietnam")
                  << trUtf8("Włochy")
                  << trUtf8("Wybrzeże Kości Słoniowej")
                  << trUtf8("Wyspy Marshalla")
                  << trUtf8("Wyspy Salomona")
                  << trUtf8("Wyspy Świętego Tomasza i Książęca")
                  << trUtf8("Zambia")
                  << trUtf8("Zimbabwe")
                  << trUtf8("Zjednoczone Emiraty Arabskie");

    setStringList(listCountries);
}

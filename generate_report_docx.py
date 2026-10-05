import zipfile
from xml.sax.saxutils import escape

OUTPUT_PATH = r"c:\Users\Hubert\Desktop\Studia PW\Przedmioty\Metody sztucznej inteligencji\Projekt 1\Raport_koncowy_v2.docx"

PARAGRAPHS = [
    {"text": "Raport ko\u0144cowy z realizacji programu", "bold": True, "center": True, "size": 32},
    {"text": "", "bold": False},
    {"text": "1. Opis struktury i g\u0142\u00f3wnych funkcji programu", "bold": True, "size": 26},
    {
        "text": "Zrealizowany program jest aplikacj\u0105 konsolow\u0105 napisan\u0105 w j\u0119zyku C++, kt\u00f3rej zadaniem jest symulacja dzia\u0142ania agenta poruszaj\u0105cego si\u0119 po planszy w kierunku drzwi ko\u0144cowych. \u015arodowisko programu jest reprezentowane w postaci mapy wczytywanej z pliku tekstowego. Na mapie wyst\u0119puj\u0105 pola puste, przeszkody, pozycja pocz\u0105tkowa agenta oraz pole celu oznaczone jako drzwi. Dzia\u0142anie programu polega na cyklicznym obserwowaniu otoczenia, wyci\u0105ganiu wniosk\u00f3w na podstawie zebranych przes\u0142anek, podejmowaniu decyzji o ruchu, a nast\u0119pnie wizualizacji aktualnego stanu planszy i zapisie historii rozumowania.",
        "bold": False,
    },
    {
        "text": "Program zosta\u0142 podzielony na kilka modu\u0142\u00f3w, co poprawia czytelno\u015b\u0107 struktury oraz rozdzielenie odpowiedzialno\u015bci. Plik main.cpp zawiera punkt wej\u015bcia programu i g\u0142\u00f3wn\u0105 p\u0119tl\u0119 steruj\u0105c\u0105. Modu\u0142 map_io odpowiada za odczyt plik\u00f3w map, wyb\u00f3r mapy przez u\u017cytkownika oraz walidacj\u0119 danych wej\u015bciowych. Modu\u0142 state przechowuje wsp\u00f3lny stan symulacji, w tym wymiary planszy, pozycj\u0119 agenta, pozycj\u0119 drzwi, rozmieszczenie przeszk\u00f3d oraz tablice pomocnicze opisuj\u0105ce ograniczenia ruchu. Modu\u0142 history zapisuje histori\u0119 obserwacji, wniosk\u00f3w i akcji agenta, a tak\u017ce udost\u0119pnia mechanizm formatowania tych informacji do postaci tekstowej. Modu\u0142 console_ui odpowiada za rysowanie planszy w konsoli i prezentacj\u0119 najnowszych wpis\u00f3w historii. Najwa\u017cniejsza logika decyzyjna zosta\u0142a umieszczona w module agent_logic, w kt\u00f3rym zaimplementowano procedury obserwacji otoczenia, wnioskowania oraz ruchu agenta.",
        "bold": False,
    },
    {
        "text": "Program zosta\u0142 rozszerzony r\u00f3wnie\u017c o mechanizm losowego generowania map. U\u017cytkownik mo\u017ce wybra\u0107 wygenerowanie mapy losowej zamiast wczytywania jednej z gotowych plansz zapisanych w katalogu maps. W takim przypadku system mo\u017ce albo samodzielnie wylosowa\u0107 parametry mapy, albo pozwoli\u0107 u\u017cytkownikowi poda\u0107 je r\u0119cznie. Losowaniu lub definiowaniu podlegaj\u0105 wymiary planszy, pozycja pocz\u0105tkowa agenta, pozycja drzwi ko\u0144cowych oraz prawdopodobie\u0144stwo wyst\u0105pienia przeszk\u00f3d. Wysoko\u015b\u0107 mapy przyjmuje warto\u015bci od 3 do 7, szeroko\u015b\u0107 od 7 do 20, agent jest zawsze umieszczany na lewej kraw\u0119dzi planszy, a drzwi na jej prawej kraw\u0119dzi. Nast\u0119pnie dla wszystkich pozosta\u0142ych p\u00f3l losowana jest obecno\u015b\u0107 przeszkody zgodnie z zadanym prawdopodobie\u0144stwem. W trybie pe\u0142nego losowania prawdopodobie\u0144stwo to jest wybierane spo\u015br\u00f3d kilku poziom\u00f3w zag\u0119szczenia przeszk\u00f3d, co pozwala uzyska\u0107 zar\u00f3wno plansze proste, jak i wyra\u017anie trudniejsze. Takie rozwi\u0105zanie umo\u017cliwia szybkie generowanie r\u00f3\u017cnorodnych \u015brodowisk testowych i wspiera praktyczn\u0105 ocen\u0119 zachowania agenta w wielu konfiguracjach przestrzennych.",
        "bold": False,
    },
    {
        "text": "G\u0142\u00f3wne funkcje programu mo\u017cna podzieli\u0107 na cztery grupy. Pierwsza grupa obejmuje funkcje zwi\u0105zane z przygotowaniem \u015brodowiska, czyli wyb\u00f3r mapy, wczytanie jej zawarto\u015bci oraz inicjalizacj\u0119 struktur danych. Druga grupa dotyczy percepcji i reprezentacji wiedzy agenta. Program rozpoznaje przeszkody i \u015bciany znajduj\u0105ce si\u0119 po prawej stronie, nad agentem i pod nim, a nast\u0119pnie zapisuje te informacje w strukturach logicznych i historii dzia\u0142ania. Trzecia grupa funkcji odpowiada za podejmowanie decyzji. Agent pr\u00f3buje porusza\u0107 si\u0119 w prawo, a je\u017celi nie jest to mo\u017cliwe, rozwa\u017ca obej\u015bcie przeszkody w d\u00f3\u0142 lub w g\u00f3r\u0119. Je\u017celi oba obej\u015bcia s\u0105 zablokowane, program dopuszcza usuni\u0119cie przeszkody znajduj\u0105cej si\u0119 na drodze. Czwarta grupa funkcji odpowiada za interfejs u\u017cytkownika i zapis wynik\u00f3w, czyli wy\u015bwietlenie stanu mapy oraz zapis pe\u0142nego dziennika dzia\u0142ania do pliku agent_log.txt.",
        "bold": False,
    },
    {"text": "2. Opis implementacji zadanej metodyki sztucznej inteligencji", "bold": True, "size": 26},
    {
        "text": "Zastosowana w programie metodyka sztucznej inteligencji ma charakter symboliczny i regu\u0142owy, a jej kluczowym elementem jest wnioskowanie w prz\u00f3d. Agent nie korzysta z uczenia maszynowego ani metod probabilistycznych, lecz opiera swoje dzia\u0142anie na jawnej reprezentacji wiedzy o otoczeniu i na zestawie regu\u0142, kt\u00f3re s\u0105 uruchamiane na podstawie aktualnie znanych fakt\u00f3w. Taka konstrukcja odpowiada klasycznemu podej\u015bciu do system\u00f3w inteligentnych, w kt\u00f3rych kluczow\u0105 rol\u0119 odgrywa baza fakt\u00f3w, mechanizm dedukcji oraz wykonywanie dzia\u0142a\u0144 wynikaj\u0105cych z aktualnego stanu wiedzy.",
        "bold": False,
    },
    {
        "text": "Implementacja metodyki zosta\u0142a oparta na trzech etapach. Pierwszym etapem jest obserwacja otoczenia. Funkcja MakeObservations analizuje pola s\u0105siaduj\u0105ce z bie\u017c\u0105c\u0105 pozycj\u0105 agenta i zapisuje spostrze\u017cenia, takie jak wykrycie przeszkody lub \u015bciany. Drugim etapem jest wnioskowanie w prz\u00f3d. Po dodaniu obserwacji funkcja InferFromLatestObservation uruchamia odpowiednie regu\u0142y, kt\u00f3re przekszta\u0142caj\u0105 aktualne fakty w bardziej u\u017cyteczn\u0105 wiedz\u0119 operacyjn\u0105, na przyk\u0142ad wniosek o braku mo\u017cliwo\u015bci ruchu w prawo, w g\u00f3r\u0119 albo w d\u00f3\u0142. Uzupe\u0142nieniem tego mechanizmu jest funkcja InferBlockedBypass, kt\u00f3ra sprawdza, czy przeszkody lub granice planszy uniemo\u017cliwiaj\u0105 obej\u015bcie blokady od g\u00f3ry i od do\u0142u. Je\u015bli oba warianty obej\u015bcia s\u0105 niemo\u017cliwe, agent wyprowadza nowy fakt m\u00f3wi\u0105cy, \u017ce mo\u017ce usun\u0105\u0107 przeszkod\u0119. W rezultacie kolejne wnioski s\u0105 wyprowadzane progresywnie z wcze\u015bniej zapisanych fakt\u00f3w, co odpowiada klasycznemu schematowi forward chaining.",
        "bold": False,
    },
    {
        "text": "Istotn\u0105 cech\u0105 tej implementacji jest to, \u017ce proces obserwacji i dedukcji nie zosta\u0142 ograniczony wy\u0142\u0105cznie do prostych operacji na tablicach logicznych. Program reprezentuje obserwacje, wnioski po\u015brednie i akcje r\u00f3wnie\u017c w postaci tekstowej, na przyk\u0142ad jako zdania typu ObstacleAt (x: 3, y: 1), Wall (x: 4, y: 0) albo CantMoveRightFrom (x: 2, y: 1). Ka\u017cda nowa obserwacja jest dodawana do historii jako fakt, a nast\u0119pnie analizowana przez mechanizm regu\u0142owy, kt\u00f3ry dopasowuje wzorce tekstowe i uruchamia odpowiednie regu\u0142y wnioskowania w prz\u00f3d. Oznacza to, \u017ce program nie tylko przechowuje stan \u015brodowiska w strukturach danych j\u0119zyka C++, ale r\u00f3wnie\u017c realizuje rzeczywisty proces dedukcyjny typu forward chaining: od faktu obserwacyjnego, przez wyprowadzenie wniosku po\u015bredniego, a\u017c do decyzji o dzia\u0142aniu. Taki spos\u00f3b implementacji dobrze odzwierciedla ide\u0119 bazy wiedzy i progresywnego wnioskowania, poniewa\u017c kolejne wnioski wynikaj\u0105 z wcze\u015bniej zapisanych zda\u0144 i mog\u0105 by\u0107 wykorzystywane w dalszych etapach rozumowania.",
        "bold": False,
    },
    {
        "text": "Trzecim etapem jest podejmowanie dzia\u0142ania. Funkcja MoveAgent realizuje strategi\u0119 sterowania zgodn\u0105 z wcze\u015bniej zgromadzon\u0105 wiedz\u0105. W pierwszej kolejno\u015bci agent usuwa przeszkod\u0119 wtedy, gdy mechanizm wnioskowania dopu\u015bci\u0142 tak\u0105 mo\u017cliwo\u015b\u0107. Nast\u0119pnie podejmowana jest pr\u00f3ba ruchu w prawo. Je\u017celi ruch w prawo jest zablokowany, agent pr\u00f3buje obej\u015b\u0107 przeszkod\u0119 ruchem w d\u00f3\u0142, a gdy ten wariant r\u00f3wnie\u017c jest niedost\u0119pny, ruchem w g\u00f3r\u0119. Ka\u017cde dzia\u0142anie jest zapisywane w historii, co umo\u017cliwia p\u00f3\u017aniejsz\u0105 analiz\u0119 przebiegu procesu decyzyjnego. W rezultacie program realizuje uproszczony model inteligentnego agenta reaguj\u0105cego na \u015brodowisko, buduj\u0105cego lokaln\u0105 wiedz\u0119 i podejmuj\u0105cego decyzje na podstawie regu\u0142 logicznych.",
        "bold": False,
    },
    {
        "text": "Istotnym elementem implementacji jest tak\u017ce rejestrowanie ca\u0142ego procesu rozumowania. W historii odr\u00f3\u017cniane s\u0105 trzy typy wpis\u00f3w: obserwacje, wnioski i akcje. Dzi\u0119ki temu mo\u017cliwe jest odtworzenie nie tylko sekwencji ruch\u00f3w, ale r\u00f3wnie\u017c przes\u0142anek, kt\u00f3re doprowadzi\u0142y agenta do okre\u015blonej decyzji. Taka forma zapisu zwi\u0119ksza przejrzysto\u015b\u0107 programu i ma znaczenie dydaktyczne, poniewa\u017c pozwala analizowa\u0107 dzia\u0142anie systemu sztucznej inteligencji krok po kroku.",
        "bold": False,
    },
    {"text": "3. Spos\u00f3b uruchomienia programu i korzystania z niego", "bold": True, "size": 26},
    {
        "text": "Program nale\u017cy skompilowa\u0107 przy u\u017cyciu kompilatora g++ z obs\u0142ug\u0105 standardu C++17. W katalogu projektu mo\u017cna wykorzysta\u0107 nast\u0119puj\u0105ce polecenie: g++ -std=c++17 -Wall -Wextra -pedantic main.cpp agent_logic.cpp console_ui.cpp history.cpp map_io.cpp state.cpp -o main.exe. Po poprawnej kompilacji program uruchamia si\u0119 poleceniem .\\main.exe.",
        "bold": False,
    },
    {
        "text": "Po uruchomieniu aplikacja wy\u015bwietla list\u0119 dost\u0119pnych map zapisanych w katalogu maps. U\u017cytkownik wybiera numer interesuj\u0105cej mapy, po czym program rozpoczyna symulacj\u0119. W kolejnych iteracjach od\u015bwie\u017cany jest widok planszy, prezentowana jest bie\u017c\u0105ca pozycja agenta, rozmieszczenie przeszk\u00f3d i drzwi oraz ostatnie wpisy historii. Program dzia\u0142a automatycznie a\u017c do chwili, w kt\u00f3rej agent osi\u0105gnie pole celu. Po zako\u0144czeniu dzia\u0142ania pe\u0142ny przebieg obserwacji, wniosk\u00f3w i akcji zostaje zapisany do pliku agent_log.txt, kt\u00f3ry mo\u017ce s\u0142u\u017cy\u0107 jako materia\u0142 do analizy lub dokumentacji eksperymentu.",
        "bold": False,
    },
    {"text": "4. Podsumowanie weryfikacji poprawnego dzia\u0142ania programu", "bold": True, "size": 26},
    {
        "text": "Weryfikacja poprawno\u015bci dzia\u0142ania programu zosta\u0142a przeprowadzona poprzez uruchomienie programu i sprawdzenie, czy agent prawid\u0142owo realizuje swoje zadanie w r\u00f3\u017cnych warunkach pracy. W trakcie test\u00f3w potwierdzono poprawne dzia\u0142anie programu dla wszystkich scenariuszy weryfikacyjnych opisanych w raporcie wst\u0119pnym. Obejmowa\u0142y one kolejno: korytarz bez przeszk\u00f3d, przypadek pojedynczych przeszk\u00f3d omijanych przez agenta, przypadek przeszk\u00f3d niemo\u017cliwych do omini\u0119cia i wymagaj\u0105cych usuwania, scenariusz mieszany zawieraj\u0105cy zar\u00f3wno przeszkody omijalne, jak i nieomijalne, przypadek ca\u0142kowitego wype\u0142nienia korytarza przeszkodami oraz przypadek przeszk\u00f3d zlokalizowanych przy ko\u0144cowej \u015bcianie korytarza, kt\u00f3re nale\u017ca\u0142o usun\u0105\u0107 w celu odnalezienia drzwi. Taki zestaw test\u00f3w odpowiada pe\u0142nemu przekrojowi sytuacji przewidzianych w za\u0142o\u017ceniach projektowych i pozwala oceni\u0107 poprawno\u015b\u0107 dzia\u0142ania systemu w warunkach prostych, po\u015brednich i skrajnych.",
        "bold": False,
    },
    {
        "text": "W przeprowadzonych testach potwierdzono, \u017ce program poprawnie wczytuje mapy, rozpoznaje po\u0142o\u017cenie agenta, przeszk\u00f3d i drzwi, wykonuje obserwacje s\u0105siednich p\u00f3l, generuje wnioski o ograniczeniach ruchu, podejmuje decyzje zgodne z zaimplementowan\u0105 logik\u0105 oraz zapisuje histori\u0119 dzia\u0142ania do pliku. Otrzymane wyniki potwierdzaj\u0105, \u017ce program zachowuje si\u0119 zgodnie z przyj\u0119tymi za\u0142o\u017ceniami i poprawnie realizuje wszystkie przewidziane przypadki dzia\u0142ania agenta.",
        "bold": False,
    },
    {
        "text": "Na podstawie przeprowadzonej weryfikacji mo\u017cna stwierdzi\u0107, \u017ce program spe\u0142nia za\u0142o\u017cone wymagania funkcjonalne. Implementacja pozwala na czytelne zaprezentowanie dzia\u0142ania prostego agenta inteligentnego wykorzystuj\u0105cego obserwacj\u0119, reprezentacj\u0119 wiedzy i regu\u0142owe wnioskowanie do podejmowania decyzji w \u015brodowisku planszowym.",
        "bold": False,
    },
]


def make_run(text, bold=False, size=None):
    props = []
    if bold:
        props.append("<w:b/>")
    if size is not None:
        props.append(f'<w:sz w:val="{size}"/><w:szCs w:val="{size}"/>')

    props_xml = f'<w:rPr>{"".join(props)}</w:rPr>' if props else ""
    return f'<w:r>{props_xml}<w:t xml:space="preserve">{escape(text)}</w:t></w:r>'


def make_paragraph(text, bold=False, center=False, size=None):
    ppr = '<w:pPr><w:jc w:val="center"/></w:pPr>' if center else ""
    if text == "":
        return f"<w:p>{ppr}</w:p>"
    return f"<w:p>{ppr}{make_run(text, bold=bold, size=size)}</w:p>"


body = "".join(make_paragraph(**paragraph) for paragraph in PARAGRAPHS)

document_xml = f"""<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<w:document xmlns:wpc="http://schemas.microsoft.com/office/word/2010/wordprocessingCanvas"
 xmlns:mc="http://schemas.openxmlformats.org/markup-compatibility/2006"
 xmlns:o="urn:schemas-microsoft-com:office:office"
 xmlns:r="http://schemas.openxmlformats.org/officeDocument/2006/relationships"
 xmlns:m="http://schemas.openxmlformats.org/officeDocument/2006/math"
 xmlns:v="urn:schemas-microsoft-com:vml"
 xmlns:wp14="http://schemas.microsoft.com/office/word/2010/wordprocessingDrawing"
 xmlns:wp="http://schemas.openxmlformats.org/drawingml/2006/wordprocessingDrawing"
 xmlns:w10="urn:schemas-microsoft-com:office:word"
 xmlns:w="http://schemas.openxmlformats.org/wordprocessingml/2006/main"
 xmlns:w14="http://schemas.microsoft.com/office/word/2010/wordml"
 xmlns:w15="http://schemas.microsoft.com/office/word/2012/wordml"
 xmlns:wpg="http://schemas.microsoft.com/office/word/2010/wordprocessingGroup"
 xmlns:wpi="http://schemas.microsoft.com/office/word/2010/wordprocessingInk"
 xmlns:wne="http://schemas.microsoft.com/office/2006/wordml"
 xmlns:wps="http://schemas.microsoft.com/office/word/2010/wordprocessingShape"
 mc:Ignorable="w14 w15 wp14">
  <w:body>
    {body}
    <w:sectPr>
      <w:pgSz w:w="11906" w:h="16838"/>
      <w:pgMar w:top="1440" w:right="1440" w:bottom="1440" w:left="1440" w:header="708" w:footer="708" w:gutter="0"/>
    </w:sectPr>
  </w:body>
</w:document>
"""

content_types = """<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<Types xmlns="http://schemas.openxmlformats.org/package/2006/content-types">
  <Default Extension="rels" ContentType="application/vnd.openxmlformats-package.relationships+xml"/>
  <Default Extension="xml" ContentType="application/xml"/>
  <Override PartName="/word/document.xml" ContentType="application/vnd.openxmlformats-officedocument.wordprocessingml.document.main+xml"/>
  <Override PartName="/docProps/core.xml" ContentType="application/vnd.openxmlformats-package.core-properties+xml"/>
  <Override PartName="/docProps/app.xml" ContentType="application/vnd.openxmlformats-officedocument.extended-properties+xml"/>
</Types>
"""

rels = """<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<Relationships xmlns="http://schemas.openxmlformats.org/package/2006/relationships">
  <Relationship Id="rId1" Type="http://schemas.openxmlformats.org/officeDocument/2006/relationships/officeDocument" Target="word/document.xml"/>
  <Relationship Id="rId2" Type="http://schemas.openxmlformats.org/package/2006/relationships/metadata/core-properties" Target="docProps/core.xml"/>
  <Relationship Id="rId3" Type="http://schemas.openxmlformats.org/officeDocument/2006/relationships/extended-properties" Target="docProps/app.xml"/>
</Relationships>
"""

core = """<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<cp:coreProperties xmlns:cp="http://schemas.openxmlformats.org/package/2006/metadata/core-properties" xmlns:dc="http://purl.org/dc/elements/1.1/" xmlns:dcterms="http://purl.org/dc/terms/" xmlns:dcmitype="http://purl.org/dc/dcmitype/" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance">
  <dc:title>Raport końcowy</dc:title>
  <dc:subject>Opis programu i weryfikacja działania</dc:subject>
  <dc:creator>Codex</dc:creator>
  <cp:lastModifiedBy>Codex</cp:lastModifiedBy>
  <dcterms:created xsi:type="dcterms:W3CDTF">2026-04-16T00:00:00Z</dcterms:created>
  <dcterms:modified xsi:type="dcterms:W3CDTF">2026-04-16T00:00:00Z</dcterms:modified>
</cp:coreProperties>
"""

app = """<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<Properties xmlns="http://schemas.openxmlformats.org/officeDocument/2006/extended-properties" xmlns:vt="http://schemas.openxmlformats.org/officeDocument/2006/docPropsVTypes">
  <Application>Microsoft Office Word</Application>
</Properties>
"""

with zipfile.ZipFile(OUTPUT_PATH, "w", compression=zipfile.ZIP_DEFLATED) as docx:
    docx.writestr("[Content_Types].xml", content_types)
    docx.writestr("_rels/.rels", rels)
    docx.writestr("docProps/core.xml", core)
    docx.writestr("docProps/app.xml", app)
    docx.writestr("word/document.xml", document_xml)

print(OUTPUT_PATH)

Del 1~10%- Spillklient
	Kommandolinje-konfigurasjoner (spillernavn, poengtjener, flerspillertjener, osv.).
		- Dette er implementert og kommentert
	Poengsummen opp/ned-lasting, løpende oppdateringer og framvisning av disse.
		- Dette er implementert og kommentert
	Støtter flerspillermodus og enkeltspillermodus.
		- Enkeltspillermodus er implementert og kommentert, men flerspillermodus har jeg desverre ikke fått til helt optimalt. Jeg fikk ikke til å vise flere spillere på skjermen, men det er mulig å koble seg til «multiplayerserveren», slik at alle de forskjellige pakketypene (start, update, setup og end) blir sendt under spilling.
Del 2~30%- Kommunikasjonspakke implementert i TCP
	Nettverkskommunikasjonen og buffere ligger gjemt i netcom.c og netcom.h slik at klient og tjenere blir oversiktlig.
		- All nettverkskommunikasjon foretas i netcom.c og netcom.h. Dette er kommentert
	Stabilitet, en klients kommunikasjon kan ikke permanent låse tjenester for andre.
		- Det skal være mulig for flere å koble til serveren og sende pakker samtidig. Dette er kommentert.
	Spillklient-til-poengtjener kommunikasjon implementer etter gitt standard.
		- Dette er implementert og kommentert
	Spillklient-til-flerspilltjener kommunikasjon.
		- Delvis implementert. Klarte ikke å lage flerspillerserver, så når en
spiller vil spille flerspiller, kobler den seg til enkeltspillerserveren, som så tar i mot
alle pakkene som trengs for å spille multiplayer. 
	Meldingsoppbygging til flerspillerpakker.
		- Dette er implementert og kommentert
	Tåler treghet i nettverket og/eller oppdelte pakker.
		- Dette er implementert og kommentert
	Sender feilmelding og lukker tilkoblinger dersom man oppdager feil eller uløselige situasjoner.
		- Dette er implementert og kommentert
Del 3~30%- Poengtjener
	Argumenter ved oppstart.
		- Dette er implementert og kommentert
	Sammenhengende minne for hverenkelt spilltype.
		- Dette er implementert og kommentert
	Effektiv uthening og overskriving av poengsummer.
		- Dette er implementert og kommentert
	Lagrer poengsummer og statistikk ved avslutting, mulighet for innlasting ved oppstart.
		- Dette er implementert og kommentert
	Kontrolert avslutting ved Ctrl+D og Ctrl+C, og printing av statistikk.
		- Dette er implementert og kommentert
Del 4~10%- Flerspillertjener
		- Dette har jeg desverre ikke fått implementert. For å få til de forskjellige pakkene som sendes fra klienten som vil spille flerspiller og vise at jeg har forstått hvordan dette fungerer, bruker jeg enkteltspillerserveren til dette.

Makefile:
	- Denne er laget slik at .o-files lages før kompilering, før disse kompileres for hhv server(highscore-server og tilhørende filer) og tjener (ifitron og tilhørende filer). Mappestrukturen er slik at alle .c og .h-filer ligger i mappen src, alle .o-filer ligger i mappen obj, mens alle executables ligger i bin-mappen.
	- For å se hvilke kommandoer som kan kjøres fra makefilen, kan sensor skrive «make info» i terminalen.

Kommentar til min besvarelse:
Kommentarene i .c- og.h-filene er skrevet på engelsk, fordi det føltes mest naturlig.
Jeg har kommentert all kode ganske grundig, fordi jeg føler det er bedre å kommentere for mye enn for lite.
Siden det ikke var noe krav, har jeg ikke sjekket om jeg kan koble opp på serveren fra en annen pc enn min egen.

- Jeg har delt opp koden i flere filer, for oversiktlighet. client_handler har funksjoner for ifitron(klienten), mens server_handler har funksjoner for serveren.  
- Poeng-pakker kalles point_packages, poenglistepakker kalles highscore_packages, og feilmeldingspakker kalles error_packages.
- Feilmeldingspakkene er implementert slik at de kan inneholde en lengre tekst med en melding om hvilken feil som oppstod. Denne sendes eksempelsvis tilbake fra server om meldingstype ikke finnes, om feltverdiene i protokollene er feil, om toerkomplementet ikke er negativ. De sendes fra spilltjener til server om det er samme feil i poenglistepakkene den mottar fra serveren. Jeg har laget en egen print_in_game funksjon for å kunne skrive feilmeldinger i spillet, og ikke i terminalen, noe som fører til at spillet låser seg.
- Av mangler til oppgaven så har jeg ikke fått implementert flerspillerdelen tilstrekkelig. Pakker sendes fra klient til server, men serveren er en enkeltspillerserver, derfor får jeg ikke sendt pakker tilbake til en annen spiller koblet på samtidig. Jeg ser for meg at pakkene jeg sender ville blitt sendt til serveren (update, end, start og setup), som tar vare på opptil to spillere før den sender noe tilbake , og sender disse pakkene tilbake til den andre spilleren slik at dette kunne blitt tegnet på den andre spillerens skjerm. Likevel ville jeg prøve å få til en del av flerspiller-delen, da blant annet disse pakkene, samt at disse blir sendt på riktig tidspunkt under spilling, og at klienten vet når den kan spille flerspiller og enkeltspiller, ved hjelp av argumentene ved oppstart. Som en følge av at serveren fungerer som en make shift multiplayerserver, får også spillere som vil spille i enkeltspiller-modus mulighet til å koble seg til, selv om disse ikke kan velge multiplayer-modus i selve spillet.
- Avslutning ved CTRL-D fungerer heller ikke optimalt, da den ikke får lagret det som er lagt til i minne etter at barneprosessen er startet. Dette er fordi jeg fork’er en barneprosess, som ikke vet noe om hva som skjer i foreldre-prosessen etter fork’ingen. Derfor vet den ikke om de nye highscorene som er lagt til minne. Toer-komplementet har jeg heller ikke fått implementert helt riktig, måten jeg gjør det på er å sjekke om verdien fra tjener er negativ, hvis ikke sendes en error-pakke tilbake. Antall scores som vises i spillet har jeg laget slik at antall minneområder som har en høyeste score, får vist sin høyeste score i spillet.  
- Har brukt valgrind for å sjekke at ingen minnelekasjer eller lesing/skriving til/fra plasser utenfor minne skjer.
- Ifitron fungerer slik at hver gang et spill starter kobles den opp mot serveren og sender den en pakke til serveren, og lukker tilkoblingen (eventuelt venter på en pakke i retur hvis det er bedt om eller det har skjedd en feil). Et spill starter når hastighet er valgt. Det er mulig å starte et spill uten at serveren er koblet til, men da blir ingenting sent til serverne (selvfølgelig). Jeg har gjort det slik for «penhetens» skyld. Fifa kan jo for eksempel spilles uten nettilkobling.
- Høyeste og laveste score for hver spilltype blir endret (hvis det er nødvendig) for hver innsetting av en ny score. Dermed kan disse effektivt hentes ut og skrives over.
# photostop
image processor written with C++ and QT

* Dupla ()
* Processamento sequencial
  * resultados usados em sequência
* Outras operações além da implementação mínima
* Bordas da imagem: replicação
* Leitura e escrita geral de PGM e PPM
  * ASCII e Binária (RAW)
* Exibir menssagens e código da execução (tela e arquivo)
* Material a entregar (detalhes posteriormente)

- [X] Identificação do cabeçalho
  - [X] Tipo
    - [X] P2 PGM ASCII (exemplo)
  - [X] Dimensão
  - [X] Valor máximo
- [ ] Filtragem espacial
  - [ ] Média (3x3, 5x5, ...)
  - [ ] Mediana (3x3, 5x5, ...)
  - [ ] Laplaciano
  - [ ] Alto reforço (high boost)
    - [ ] Filtro da média específicada
  - [ ] Equalização de histograma global
  - [ ] Extração e junção RGB

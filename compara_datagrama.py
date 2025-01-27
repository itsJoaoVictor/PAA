import os

def ler_arquivo(caminho):
    """Lê o conteúdo de um arquivo e retorna uma lista de linhas."""
    if not os.path.exists(caminho):
        print(f"Erro: Arquivo '{caminho}' não encontrado.")
        return None
    with open(caminho, 'r', encoding='utf-8') as arquivo:
        return [linha.strip() for linha in arquivo.readlines()]

def comparar_saidas(arquivo_gerado, arquivo_esperado):
    """Compara o conteúdo de dois arquivos linha por linha."""
    gerado = ler_arquivo(arquivo_gerado)
    esperado = ler_arquivo(arquivo_esperado)

    if gerado is None or esperado is None:
        return

    if len(gerado) != len(esperado):
        print(f"Diferença encontrada: número de linhas diferente.")
        print(f"Arquivo gerado tem {len(gerado)} linhas e o esperado tem {len(esperado)} linhas.")
        return

    diferencas = []
    for i, (linha_gerada, linha_esperada) in enumerate(zip(gerado, esperado)):
        if linha_gerada != linha_esperada:
            diferencas.append((i + 1, linha_gerada, linha_esperada))

    if not diferencas:
        print("Os arquivos são idênticos.")
    else:
        print("Diferenças encontradas:")
        for linha, gerado, esperado in diferencas:
            print(f"Linha {linha}:")
            print(f"  Gerado:  {gerado}")
            print(f"  Esperado: {esperado}")

if __name__ == "__main__":
    # Caminhos para os arquivos
    arquivo_gerado = r"C:\Users\Pichau\Codigos\Projetos UFS\PAA\output\datagrama.output"
    arquivo_esperado = r"C:\Users\Pichau\Codigos\Projetos UFS\PAA\datagramaesperado.output"

    # Comparar os arquivos
    comparar_saidas(arquivo_gerado, arquivo_esperado)

import sys

def carregar_arquivo(caminho):
    with open(caminho, "r", encoding="utf-8") as f:
        return [linha.strip() for linha in f.readlines()]

def comparar_arquivos(arquivo_saida, arquivo_desejado):
    saida = carregar_arquivo(arquivo_saida)
    desejado = carregar_arquivo(arquivo_desejado)
    
    # Abrir o arquivo de diferenças no início da execução para garantir que o arquivo está disponível
    with open("diferencas.txt", "a", encoding="utf-8") as f:
        if len(saida) != len(desejado):
            f.write(f"Diferença no número de linhas: esperado {len(desejado)}, obtido {len(saida)}\n")
            print(f"Diferença no número de linhas: esperado {len(desejado)}, obtido {len(saida)}")
        
        for i, (linha_saida, linha_desejada) in enumerate(zip(saida, desejado), start=1):
            if linha_saida != linha_desejada:
                f.write(f"Linha {i} divergente:\n")
                f.write(f"  Esperado: {linha_desejada}\n")
                f.write(f"  Obtido  : {linha_saida}\n")
                print(f"Linha {i} divergente:")
                print(f"  Esperado: {linha_desejada}")
                print(f"  Obtido  : {linha_saida}")
        
        if saida == desejado:
            f.write("Os arquivos são idênticos.\n")
            print("Os arquivos são idênticos.")

if __name__ == "__main__":
    arquivo_saida = "C:\\Users\\Pichau\\Codigos\\Projetos UFS\\PAA\\output\\porto.output"
    arquivo_desejado = "C:\\Users\\Pichau\\Codigos\\Projetos UFS\\PAA\\porto.output"  # O mesmo arquivo desejado
    comparar_arquivos(arquivo_saida, arquivo_desejado)

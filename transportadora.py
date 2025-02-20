#!/usr/bin/env python3
# -*- coding: utf-8 -*-

def ler_arquivo(caminho):
    """
    Lê o arquivo no caminho especificado e retorna uma lista com as linhas.
    """
    with open(caminho, 'r', encoding='utf-8') as f:
        # Remove quebras de linha ao final de cada linha para facilitar a comparação
        linhas = [linha.rstrip('\n') for linha in f.readlines()]
    return linhas

def comparar_arquivos(arq1, arq2):
    """
    Compara os dois arquivos linha a linha e exibe as diferenças.
    """
    linhas1 = ler_arquivo(arq1)
    linhas2 = ler_arquivo(arq2)

    max_linhas = max(len(linhas1), len(linhas2))
    diferencas = False

    for i in range(max_linhas):
        # Pega a linha i de cada arquivo, se existir
        linha1 = linhas1[i] if i < len(linhas1) else ''
        linha2 = linhas2[i] if i < len(linhas2) else ''
        
        if linha1 != linha2:
            diferencas = True
            print(f"Linha {i+1}:")
            print(f"  Sua saída     : {linha1}")
            print(f"  Saída desejada: {linha2}")
            print("-" * 40)
    
    if not diferencas:
        print("As saídas são idênticas!")
    else:
        print("Foram encontradas diferenças entre os arquivos.")

def main():
    # Defina os caminhos dos arquivos
    caminho_minha_saida = r"C:\Users\Pichau\Codigos\Projetos UFS\PAA\output\transportadora.output"
    caminho_saida_desejada = r"C:\Users\Pichau\Codigos\Projetos UFS\PAA\transportadoraDesejado.output"
    
    comparar_arquivos(caminho_minha_saida, caminho_saida_desejada)

if __name__ == '__main__':
    main()

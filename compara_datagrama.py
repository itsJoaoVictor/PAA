import re

# Caminhos dos arquivos
arquivo1 = r"C:\Users\Pichau\Codigos\Projetos UFS\PAA\output\datagrama.output"
arquivo2 = r"C:\Users\Pichau\Codigos\Projetos UFS\PAA\datagramaesperado.output"

# Abrir e ler os arquivos
with open(arquivo1, 'r') as f1, open(arquivo2, 'r') as f2:
    conteudo1 = f1.readlines()
    conteudo2 = f2.readlines()

# Função para extrair valores de cada grupo no formato específico
def extrair_valores_datagrama(linha):
    padrao = r"\|([^|]*)\|([^|]*)\|"  # Captura grupos dentro dos pares de pipes
    # Dividir a string capturada por vírgula e remover espaços em branco
    return [part.strip().split(',') for part in re.split(r"\|\||\|", linha) if part]

# Comparar os conteúdos
if conteudo1 == conteudo2:
    print("✅ Os arquivos são iguais!")
else:
    print("❌ Os arquivos são diferentes!\n")
    print("Diferenças encontradas:")
    print("-" * 50)

    for i, (linha1, linha2) in enumerate(zip(conteudo1, conteudo2)):
        valores1 = extrair_valores_datagrama(linha1.strip())
        valores2 = extrair_valores_datagrama(linha2.strip())
        
        if valores1 == valores2:
            print(f"✅ Linha {i}: Todos os valores estão corretos.")
        else:
            print(f"❌ Linha {i}: Diferenças detectadas.")
            
            # Comparar cada grupo individualmente
            for j, (bloco1, bloco2) in enumerate(zip(valores1, valores2)):
                for valor1, valor2 in zip(bloco1, bloco2):
                    if valor1 == valor2:
                        print(f"   ✅ {valor1}")
                    else:
                        print(f"   ❌ {valor1} ➡️ {valor2}")

        # Verificar ordenação dentro da linha 1 (saída gerada)
        valores_lista1 = sum(valores1, [])
        if valores_lista1 != sorted(valores_lista1):
            print(f"   ⚠️ Linha {i} (arquivo gerado): Valores fora de ordem! {valores_lista1}")

        # Verificar ordenação dentro da linha 2 (saída esperada)
        valores_lista2 = sum(valores2, [])
        if valores_lista2 != sorted(valores_lista2):
            print(f"   ⚠️ Linha {i} (arquivo esperado): Valores fora de ordem! {valores_lista2}")

    # Para o caso de arquivos com mais linhas
    if len(conteudo1) > len(conteudo2):
        print("\n⚠️ Linhas extras no arquivo gerado:")
        for linha in conteudo1[len(conteudo2):]:
            print(f"   {linha.strip()}")

    if len(conteudo2) > len(conteudo1):
        print("\n⚠️ Linhas extras no arquivo esperado:")
        for linha in conteudo2[len(conteudo1):]:
            print(f"   {linha.strip()}")


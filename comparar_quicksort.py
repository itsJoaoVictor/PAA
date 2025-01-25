import re

# Caminhos dos arquivos
arquivo1 = r"C:\Users\Pichau\Codigos\Projetos UFS\PAA\output\quicksort.output"
arquivo2 = r"C:\Users\Pichau\Codigos\Projetos UFS\PAA\quicksortoutputesperado.output"

# Abrir e ler os arquivos
with open(arquivo1, 'r') as f1, open(arquivo2, 'r') as f2:
    conteudo1 = f1.readlines()
    conteudo2 = f2.readlines()

# Função para extrair valores de cada grupo (LP, LM, etc.)
def extrair_valores(linha):
    padrao = r"([A-Z]+)\((\d+)\)"  # Captura grupos como LM(13), LP(15), etc.
    return {tipo: int(valor) for tipo, valor in re.findall(padrao, linha)}

# Comparar os conteúdos
if conteudo1 == conteudo2:
    print("✅ Os arquivos são iguais!")
else:
    print("❌ Os arquivos são diferentes!\n")
    print("Diferenças encontradas:")
    print("-" * 50)

    for i, (linha1, linha2) in enumerate(zip(conteudo1, conteudo2)):
        valores1 = extrair_valores(linha1)
        valores2 = extrair_valores(linha2)
        
        if valores1 == valores2:
            print(f"✅ Linha {i}: Todos os valores estão corretos.")
        else:
            print(f"❌ Linha {i}: Diferenças detectadas.")
            
            # Comparar cada grupo individualmente
            for chave in sorted(set(valores1.keys()).union(valores2.keys())):
                v1 = valores1.get(chave)
                v2 = valores2.get(chave)
                if v1 == v2:
                    print(f"   ✅ {chave}: {v1}")
                else:
                    print(f"   ❌ {chave}: {v1} ➡️ {v2}")

        # Verificar ordenação dentro da linha 1 (saída gerada)
        valores_lista1 = list(valores1.values())
        if valores_lista1 != sorted(valores_lista1):
            print(f"   ⚠️ Linha {i} (arquivo gerado): Valores fora de ordem! {valores_lista1}")

        # Verificar ordenação dentro da linha 2 (saída esperada)
        valores_lista2 = list(valores2.values())
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

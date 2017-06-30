///
///  @file decodificador.c
///  @headerfile decodificador.h "decodificador.h"
///
///  @authors
///  Allisson Barros         12/0055619\n
///  Daniel Luz              13/0007714\n
///  Luiz Fernando Vieira    13/0013757\n
///  Mariana Pannunzio       12/0018276\n
///  Mateus Denucci          12/0053080\n
///
///  @date 28/06/2017
///
///  @copyright Copyright © 2017 GrupoSB. All rights reserved.
///
///  @brief Recebe uma estrutura Decodificador vazia e preenche
///		com strings que sao as instrucoes presentes no bytecode.
///		Retorna uma estrutura Decodificador preenchida.

#include "decodificador.h"

///
/// Inicializa uma estrutura Decodificador com 
/// o nome das instrucoes e a quantidade de bytes 
/// que ela ocupa.
///
/// @param Decodificador Um estrutura de Decodificador vazia
/// @return @c void
void inicializaDecodificador(Decodificador decodificador[]) {

    strcpy(decodificador[0].instrucao, "nop");
    decodificador[0].bytes = 0;

    strcpy(decodificador[1].instrucao, "aconst_null");
    decodificador[1].bytes = 0;

    strcpy(decodificador[2].instrucao, "iconst_m1");
    decodificador[2].bytes = 0;

    strcpy(decodificador[3].instrucao, "iconst_0");
    decodificador[3].bytes = 0;

    strcpy(decodificador[4].instrucao, "iconst_1");
    decodificador[4].bytes = 0;

    strcpy(decodificador[5].instrucao, "iconst_2");
    decodificador[5].bytes = 0;

    strcpy(decodificador[6].instrucao, "iconst_3");
    decodificador[6].bytes = 0;

    strcpy(decodificador[7].instrucao, "iconst_4");
    decodificador[7].bytes = 0;

    strcpy(decodificador[8].instrucao, "iconst_5");
    decodificador[8].bytes = 0;

    strcpy(decodificador[9].instrucao, "lconst_0");
    decodificador[9].bytes = 0;

    strcpy(decodificador[10].instrucao, "lconst_1");
    decodificador[10].bytes = 0;

    strcpy(decodificador[11].instrucao, "fconst_0");
    decodificador[11].bytes = 0;

    strcpy(decodificador[12].instrucao, "fconst_1");
    decodificador[12].bytes = 0;

    strcpy(decodificador[13].instrucao, "fconst_2");
    decodificador[13].bytes = 0;

    strcpy(decodificador[14].instrucao, "dconst_0");
    decodificador[14].bytes = 0;

    strcpy(decodificador[15].instrucao, "dconst_1");
    decodificador[15].bytes = 0;

    strcpy(decodificador[16].instrucao, "bipush");
    decodificador[16].bytes = 1;

    strcpy(decodificador[17].instrucao, "sipush");
    decodificador[17].bytes = 2;

    strcpy(decodificador[18].instrucao, "ldc");
    decodificador[18].bytes = 1;

    strcpy(decodificador[19].instrucao, "ldc_w");
    decodificador[19].bytes = 2;

    strcpy(decodificador[20].instrucao, "ldc2_w");
    decodificador[20].bytes = 2;

    strcpy(decodificador[21].instrucao, "iload");
    decodificador[21].bytes = 1;

    strcpy(decodificador[22].instrucao, "lload");
    decodificador[22].bytes = 1;

    strcpy(decodificador[23].instrucao, "fload");
    decodificador[23].bytes = 1;

    strcpy(decodificador[24].instrucao, "dload");
    decodificador[24].bytes = 1;

    strcpy(decodificador[25].instrucao, "aload");
    decodificador[25].bytes = 1;

    strcpy(decodificador[26].instrucao, "iload_0");
    decodificador[26].bytes = 0;

    strcpy(decodificador[27].instrucao, "iload_1");
    decodificador[27].bytes = 0;

    strcpy(decodificador[28].instrucao, "iload_2");
    decodificador[28].bytes = 0;

    strcpy(decodificador[29].instrucao, "iload_3");
    decodificador[29].bytes = 0;

    strcpy(decodificador[30].instrucao, "lload_0");
    decodificador[30].bytes = 0;

    strcpy(decodificador[31].instrucao, "lload_1");
    decodificador[31].bytes = 0;

    strcpy(decodificador[32].instrucao, "lload_2");
    decodificador[32].bytes = 0;

    strcpy(decodificador[33].instrucao, "lload_3");
    decodificador[33].bytes = 0;

    strcpy(decodificador[34].instrucao, "fload_0");
    decodificador[34].bytes = 0;

    strcpy(decodificador[35].instrucao, "fload_1");
    decodificador[35].bytes = 0;

    strcpy(decodificador[36].instrucao, "fload_2");
    decodificador[36].bytes = 0;

    strcpy(decodificador[37].instrucao, "fload_3");
    decodificador[37].bytes = 0;

    strcpy(decodificador[38].instrucao, "dload_0");
    decodificador[38].bytes = 0;

    strcpy(decodificador[39].instrucao, "dload_1");
    decodificador[39].bytes = 0;

    strcpy(decodificador[40].instrucao, "dload_2");
    decodificador[40].bytes = 0;

    strcpy(decodificador[41].instrucao, "dload_3");
    decodificador[41].bytes = 0;

    strcpy(decodificador[42].instrucao, "aload_0");
    decodificador[42].bytes = 0;

    strcpy(decodificador[43].instrucao, "aload_1");
    decodificador[43].bytes = 0;

    strcpy(decodificador[44].instrucao, "aload_2");
    decodificador[44].bytes = 0;

    strcpy(decodificador[45].instrucao, "aload_3");
    decodificador[45].bytes = 0;

    strcpy(decodificador[46].instrucao, "iaload");
    decodificador[46].bytes = 0;

    strcpy(decodificador[47].instrucao, "laload");
    decodificador[47].bytes = 0;

    strcpy(decodificador[48].instrucao, "faload");
    decodificador[48].bytes = 0;

    strcpy(decodificador[49].instrucao, "daload");
    decodificador[49].bytes = 0;

    strcpy(decodificador[50].instrucao, "aaload");
    decodificador[50].bytes = 0;

    strcpy(decodificador[51].instrucao, "baload");
    decodificador[51].bytes = 0;

    strcpy(decodificador[52].instrucao, "caload");
    decodificador[52].bytes = 0;

    strcpy(decodificador[53].instrucao, "saload");
    decodificador[53].bytes = 0;

    strcpy(decodificador[54].instrucao, "istore");
    decodificador[54].bytes = 1;

    strcpy(decodificador[55].instrucao, "lstore");
    decodificador[55].bytes = 1;

    strcpy(decodificador[56].instrucao, "fstore");
    decodificador[56].bytes = 1;

    strcpy(decodificador[57].instrucao, "dstore");
    decodificador[57].bytes = 1;

    strcpy(decodificador[58].instrucao, "astore");
    decodificador[58].bytes = 1;

    strcpy(decodificador[59].instrucao, "istore_0");
    decodificador[59].bytes = 0;

    strcpy(decodificador[60].instrucao, "istore_1");
    decodificador[60].bytes = 0;

    strcpy(decodificador[61].instrucao, "istore_2");
    decodificador[61].bytes = 0;

    strcpy(decodificador[62].instrucao, "istore_3");
    decodificador[62].bytes = 0;

    strcpy(decodificador[63].instrucao, "lstore_0");
    decodificador[63].bytes = 0;

    strcpy(decodificador[64].instrucao, "lstore_1");
    decodificador[64].bytes = 0;

    strcpy(decodificador[65].instrucao, "lstore_2");
    decodificador[65].bytes = 0;

    strcpy(decodificador[66].instrucao, "lstore_3");
    decodificador[66].bytes = 0;

    strcpy(decodificador[67].instrucao, "fstore_0");
    decodificador[67].bytes = 0;

    strcpy(decodificador[68].instrucao, "fstore_1");
    decodificador[68].bytes = 0;

    strcpy(decodificador[69].instrucao, "fstore_2");
    decodificador[69].bytes = 0;

    strcpy(decodificador[70].instrucao, "fstore_3");
    decodificador[70].bytes = 0;

    strcpy(decodificador[71].instrucao, "dstore_0");
    decodificador[71].bytes = 0;

    strcpy(decodificador[72].instrucao, "dstore_1");
    decodificador[72].bytes = 0;

    strcpy(decodificador[73].instrucao, "dstore_2");
    decodificador[73].bytes = 0;

    strcpy(decodificador[74].instrucao, "dstore_3");
    decodificador[74].bytes = 0;

    strcpy(decodificador[75].instrucao, "astore_0");
    decodificador[75].bytes = 0;

    strcpy(decodificador[76].instrucao, "astore_1");
    decodificador[76].bytes = 0;

    strcpy(decodificador[77].instrucao, "astore_2");
    decodificador[77].bytes = 0;

    strcpy(decodificador[78].instrucao, "astore_3");
    decodificador[78].bytes = 0;

    strcpy(decodificador[79].instrucao, "iastore");
    decodificador[79].bytes = 0;

    strcpy(decodificador[80].instrucao, "lastore");
    decodificador[80].bytes = 0;

    strcpy(decodificador[81].instrucao, "fastore");
    decodificador[81].bytes = 0;

    strcpy(decodificador[82].instrucao, "dastore");
    decodificador[82].bytes = 0;

    strcpy(decodificador[83].instrucao, "aastore");
    decodificador[83].bytes = 0;

    strcpy(decodificador[84].instrucao, "bastore");
    decodificador[84].bytes = 0;

    strcpy(decodificador[85].instrucao, "castore");
    decodificador[85].bytes = 0;

    strcpy(decodificador[86].instrucao, "sastore");
    decodificador[86].bytes = 0;

	strcpy(decodificador[87].instrucao, "pop");
	decodificador[87].bytes = 0;

	strcpy(decodificador[88].instrucao, "pop2");
	decodificador[88].bytes = 0;

	strcpy(decodificador[89].instrucao, "dup");
	decodificador[89].bytes = 0;

	strcpy(decodificador[90].instrucao, "dup_x1");
	decodificador[90].bytes = 0;

	strcpy(decodificador[91].instrucao, "dup_x2");
	decodificador[91].bytes = 0;

	strcpy(decodificador[92].instrucao, "dup2");
	decodificador[92].bytes = 0;

	strcpy(decodificador[93].instrucao, "dup2_x1");
	decodificador[93].bytes = 0;

	strcpy(decodificador[94].instrucao, "dup2_x2");
	decodificador[94].bytes = 0;

	strcpy(decodificador[95].instrucao, "swap");
	decodificador[95].bytes = 0;

	strcpy(decodificador[96].instrucao, "iadd");
	decodificador[96].bytes = 0;

	strcpy(decodificador[97].instrucao, "ladd");
	decodificador[97].bytes = 0;

	strcpy(decodificador[98].instrucao, "fadd");
	decodificador[98].bytes = 0;

	strcpy(decodificador[99].instrucao, "dadd");
	decodificador[99].bytes = 0;

	strcpy(decodificador[100].instrucao, "isub");
	decodificador[100].bytes = 0;

	strcpy(decodificador[101].instrucao, "lsub");
	decodificador[101].bytes = 0;

	strcpy(decodificador[102].instrucao, "fsub");
	decodificador[102].bytes = 0;

	strcpy(decodificador[103].instrucao, "dsub");
	decodificador[103].bytes = 0;

	strcpy(decodificador[104].instrucao, "imul");
	decodificador[104].bytes = 0;

	strcpy(decodificador[105].instrucao, "lmul");
	decodificador[105].bytes = 0;

	strcpy(decodificador[106].instrucao, "fmul");
	decodificador[106].bytes = 0;

	strcpy(decodificador[107].instrucao, "dmul");
	decodificador[107].bytes = 0;

	strcpy(decodificador[108].instrucao, "idiv");
	decodificador[108].bytes = 0;

	strcpy(decodificador[109].instrucao, "ldiv");
	decodificador[109].bytes = 0;

	strcpy(decodificador[110].instrucao, "fdiv");
	decodificador[110].bytes = 0;

	strcpy(decodificador[111].instrucao, "ddiv");
	decodificador[111].bytes = 0;

	strcpy(decodificador[112].instrucao, "irem");
	decodificador[112].bytes = 0;

	strcpy(decodificador[113].instrucao, "lrem");
	decodificador[113].bytes = 0;

	strcpy(decodificador[114].instrucao, "frem");
	decodificador[114].bytes = 0;

	strcpy(decodificador[115].instrucao, "drem");
	decodificador[115].bytes = 0;

	strcpy(decodificador[116].instrucao, "ineg");
	decodificador[116].bytes = 0;

	strcpy(decodificador[117].instrucao, "lneg");
	decodificador[117].bytes = 0;

	strcpy(decodificador[118].instrucao, "fneg");
	decodificador[118].bytes = 0;

	strcpy(decodificador[119].instrucao, "dneg");
	decodificador[119].bytes = 0;

	strcpy(decodificador[120].instrucao, "ishl");
	decodificador[120].bytes = 0;

	strcpy(decodificador[121].instrucao, "lshl");
	decodificador[121].bytes = 0;

	strcpy(decodificador[122].instrucao, "ishr");
	decodificador[122].bytes = 0;

	strcpy(decodificador[123].instrucao, "lshr");
	decodificador[123].bytes = 0;

	strcpy(decodificador[124].instrucao, "iushr");
	decodificador[124].bytes = 0;

	strcpy(decodificador[125].instrucao, "lushr");
	decodificador[125].bytes = 0;

	strcpy(decodificador[126].instrucao, "iand");
	decodificador[126].bytes = 0;

	strcpy(decodificador[127].instrucao, "land");
	decodificador[127].bytes = 0;

	strcpy(decodificador[128].instrucao, "ior");
	decodificador[128].bytes = 0;

	strcpy(decodificador[129].instrucao, "lor");
	decodificador[129].bytes = 0;

	strcpy(decodificador[130].instrucao, "ixor");
	decodificador[130].bytes = 0;

	strcpy(decodificador[131].instrucao, "lxor");
	decodificador[131].bytes = 0;

	strcpy(decodificador[132].instrucao, "iinc");
	decodificador[132].bytes = 2;

	strcpy(decodificador[133].instrucao, "i2l");
	decodificador[133].bytes = 0;

    strcpy(decodificador[134].instrucao, "i2f");
    decodificador[134].bytes = 0;

    strcpy(decodificador[135].instrucao, "i2d");
    decodificador[135].bytes = 0;

    strcpy(decodificador[136].instrucao, "l2i");
    decodificador[136].bytes = 0;

    strcpy(decodificador[137].instrucao, "l2f");
    decodificador[137].bytes = 0;

    strcpy(decodificador[138].instrucao, "l2d");
    decodificador[138].bytes = 0;

    strcpy(decodificador[139].instrucao, "f2i");
    decodificador[139].bytes = 0;

    strcpy(decodificador[140].instrucao, "f2l");
    decodificador[140].bytes = 0;

    strcpy(decodificador[141].instrucao, "f2d");
    decodificador[141].bytes = 0;

    strcpy(decodificador[142].instrucao, "d2i");
    decodificador[142].bytes = 0;

    strcpy(decodificador[143].instrucao, "d2l");
    decodificador[143].bytes = 0;

    strcpy(decodificador[144].instrucao, "d2f");
    decodificador[144].bytes = 0;

    strcpy(decodificador[145].instrucao, "i2b");
    decodificador[145].bytes = 0;

    strcpy(decodificador[146].instrucao, "i2c");
    decodificador[146].bytes = 0;

    strcpy(decodificador[147].instrucao, "i2s");
    decodificador[147].bytes = 0;

    strcpy(decodificador[148].instrucao, "lcmp");
    decodificador[148].bytes = 0;

    strcpy(decodificador[149].instrucao, "fcmpl");
    decodificador[149].bytes = 0;

    strcpy(decodificador[150].instrucao, "fcmpg");
    decodificador[150].bytes = 0;

    strcpy(decodificador[151].instrucao, "dcmpl");
    decodificador[151].bytes = 0;

    strcpy(decodificador[152].instrucao, "dcmpg");
    decodificador[152].bytes = 0;

    strcpy(decodificador[153].instrucao, "ifeq");
    decodificador[153].bytes = 2;

    strcpy(decodificador[154].instrucao, "ifne");
    decodificador[154].bytes = 2;

    strcpy(decodificador[155].instrucao, "iflt");
    decodificador[155].bytes = 2;

    strcpy(decodificador[156].instrucao, "ifge");
    decodificador[156].bytes = 2;

    strcpy(decodificador[157].instrucao, "ifgt");
    decodificador[157].bytes = 2;

    strcpy(decodificador[158].instrucao, "ifle");
    decodificador[158].bytes = 2;

    strcpy(decodificador[159].instrucao, "if_icmpeq");
    decodificador[159].bytes = 2;

    strcpy(decodificador[160].instrucao, "if_icmpne");
    decodificador[160].bytes = 2;

    strcpy(decodificador[161].instrucao, "if_icmplt");
    decodificador[161].bytes = 2;

    strcpy(decodificador[162].instrucao, "if_icmpge");
    decodificador[162].bytes = 2;

    strcpy(decodificador[163].instrucao, "if_icmpgt");
    decodificador[163].bytes = 2;

    strcpy(decodificador[164].instrucao, "if_icmple");
    decodificador[164].bytes = 2;

    strcpy(decodificador[165].instrucao, "if_acmpeq");
    decodificador[165].bytes = 2;

    strcpy(decodificador[166].instrucao, "if_acmpne");
    decodificador[166].bytes = 2;

    strcpy(decodificador[167].instrucao, "goto");
    decodificador[167].bytes = 2;

    strcpy(decodificador[168].instrucao, "jsr");
    decodificador[168].bytes = 2;

    strcpy(decodificador[169].instrucao, "ret");
    decodificador[169].bytes = 1;

    strcpy(decodificador[170].instrucao, "tableswitch");
    decodificador[170].bytes = 14;

    strcpy(decodificador[171].instrucao, "lookupswitch");
    decodificador[171].bytes = 10;

    strcpy(decodificador[172].instrucao, "ireturn");
    decodificador[172].bytes = 0;

    strcpy(decodificador[173].instrucao, "lreturn");
    decodificador[173].bytes = 0;

    strcpy(decodificador[174].instrucao, "freturn");
    decodificador[174].bytes = 0;

    strcpy(decodificador[175].instrucao, "dreturn");
    decodificador[176].bytes = 0;

    strcpy(decodificador[176].instrucao, "areturn");
    decodificador[176].bytes = 0;

    strcpy(decodificador[177].instrucao, "return");
    decodificador[177].bytes = 0;

    strcpy(decodificador[178].instrucao, "getstatic");
    decodificador[178].bytes = 2;

    strcpy(decodificador[179].instrucao, "putstatic");
    decodificador[179].bytes = 2;

    strcpy(decodificador[180].instrucao, "getfield");
    decodificador[180].bytes = 2;

    strcpy(decodificador[181].instrucao, "putfield");
    decodificador[181].bytes = 2;

    strcpy(decodificador[182].instrucao, "invokevirtual");
    decodificador[182].bytes = 2;

    strcpy(decodificador[183].instrucao, "invokespecial");
    decodificador[183].bytes = 2;

    strcpy(decodificador[184].instrucao, "invokestatic");
    decodificador[184].bytes = 2;

    strcpy(decodificador[185].instrucao, "invokeinterface");
    decodificador[185].bytes = 4;

    strcpy(decodificador[186].instrucao, "invokedynamic");
    decodificador[186].bytes = 4;

    strcpy(decodificador[187].instrucao, "new");
    decodificador[187].bytes = 2;

    strcpy(decodificador[188].instrucao, "newarray");
    decodificador[188].bytes = 1;

    strcpy(decodificador[189].instrucao, "anewarray");
    decodificador[189].bytes = 2;

    strcpy(decodificador[190].instrucao, "arraylength");
    decodificador[190].bytes = 0;

    strcpy(decodificador[191].instrucao, "athrow");
    decodificador[191].bytes = 0;

    strcpy(decodificador[192].instrucao, "checkcast");
    decodificador[192].bytes = 2;

    strcpy(decodificador[193].instrucao, "instanceof");
    decodificador[193].bytes = 2;

    strcpy(decodificador[194].instrucao, "monitorenter");
    decodificador[194].bytes = 0;

    strcpy(decodificador[195].instrucao, "monitorexit");
    decodificador[195].bytes = 0;


    decodificador[196].bytes = 3;

    strcpy(decodificador[197].instrucao, "multianewarray");
    decodificador[197].bytes = 3;

    strcpy(decodificador[198].instrucao, "ifnull");
    decodificador[198].bytes = 2;

    strcpy(decodificador[199].instrucao, "ifnonnull");
    decodificador[199].bytes = 2;

    strcpy(decodificador[200].instrucao, "goto_w");
    decodificador[200].bytes = 4;

    strcpy(decodificador[201].instrucao, "jsr_w");
    decodificador[201].bytes = 4;

    //codigos reservados
    strcpy(decodificador[202].instrucao, "breakpoint");
    decodificador[202].bytes = 0;

    strcpy(decodificador[254].instrucao, "impdep1");
    decodificador[254].bytes = 0;

    strcpy(decodificador[255].instrucao, "impdep2");
    decodificador[255].bytes = 0;
}

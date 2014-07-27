#ifndef	__DEBUG_RESULT_H__ONE__
#define	__DEBUG_RESULT_H__ONE__

// どんな結果についてか
enum	RESULT_TYPE{
	RESULT_TYPE_INITIALIZE,	// 初期化
	RESULT_TYPE_FINALIZE,	// 終了
	RESULT_TYPE_CREATE,		// 作成(なんでも)
	RESULT_TYPE_MAP,		// マッピング
	RESULT_TYPE_LOAD,		// 読み込み
	RESULT_TYPE_UPDATE,		// 更新(いらないかも)
	RESULT_TYPE_COMPILE,	// コンパイル
	RESULT_TYPE_SET,		// 設定
	RESULT_TYPE_DELETE,		// 削除
	RESULT_TYPE_COMPLATE	// その処理群が完了したか
};

//struct	_RESULT
//{
//	RESULT_TYPE	type;
//	bool		isSuccess;	// 成功したか失敗したか
//}

#endif
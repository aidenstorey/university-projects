//
// c++ XML Parser Library
//
// (c) Josh Gibbs
//
#ifndef GUARD_PARSENODEMANAGER_H_1378285311l
#define GUARD_PARSENODEMANAGER_H_1378285311l

// Library includes
#include <stdio.h>

#define	PARSENODE_INVALID			-1

#define MINIMUM_NODE_COUNT			16
#define NODE_BLOCK_EXPAND_SIZE		16

#define MAX_ERROR_STRING_SIZE		256
#define ERROR_CONTEXT_STRING_SIZE	64
#define ERROR_CONTEXT_PREFIX_LEN	16

enum
{
	NODE_RETURN_SET_NONE,
	NODE_RETURN_SET_ALL
};

enum
{
	NODE_ARRAY_SUCCESS,
	NODE_ARRAY_FAILURE,
	NODE_ARRAY_FAILURE_OUT_OF_MEMORY
};

#ifndef REWRITE_BUFFER_GROW_SIZE
	#define REWRITE_BUFFER_GROW_SIZE 1024
#endif

class CParseNode
{
public:
	friend class CParseNodeManager;
public:
	CParseNode();
	~CParseNode();
	void	ResetNode();
	void	InitNode(int NodeOperator, char *pszStringStart);

	int   GetNodeOperator() const;
	char *GetNodeString() const;
	char *GetNodeStringEnd() const;
	long  GetNodeStringLength() const;
	bool	GetNodeChildAdjustFlag();

	void	SetCustomData(void *pData);
	void	*GetCustomData();

	bool	SetOverrideString( const char* pszNewString /* Note: Using this will not affect the parse information.  It is only useful for rewriting modified information back out from the parse */);
	char	*GetOverrideString();

	char	GetTerminationCharacter();

	// String helper functions that can be used without needing to run TerminateStrings
	int		node_strcmp(char *pszString);
	int		node_stricmp(char *pszString);
	void	node_strcat(char *pszDestString);

	CParseNode	*GetNext() const;
	CParseNode	*GetPrevious() const;
	CParseNode	*GetChildHead() const;
	CParseNode	*GetChildTail() const;

	bool	OutputData(char *pszFilename);	// Writes the node to file, including any changes made to child nodes
	char	*OutputData(FILE *hFileHandle, char *pszBufferedDataStart, char *pszBufferedDataEnd);
	char	*OutputData(bool *bMemoryFail, char **ppszTargetString, size_t *nTargetStringLength, size_t *nTargetStringBufferSize, char *pszBufferedDataStart, char *pszBufferedDataEnd);
	char	*OutputDataWriteToBuffer(bool *bMemoryFail, char **ppszTargetString, size_t *nTargetStringLength, size_t *nTargetStringBufferSize, char *pszStringToAttach, size_t nAttachLength);

private:
	void	*m_pCustomData;

	void	AddChildNode(CParseNode *pNode, CParseNode *pInsertBefore = NULL);

	int		 m_NodeOperator;
	char	*m_pszStringStart;
	char	*m_pszStringFinish;
	long	 m_StringLength;

	bool	m_bChildAdjusted;	// Indicates that a child node has been modified since parsing, so a rewrite must be done from a lower level
	char	*m_pszOverrideString;	// Used when inserting new information into parse tree for rewrite
	size_t	m_nOverrideStringLength;

	char	m_nTerminationCharacter;	// The character that follows this node, for use when untermination is required

	CParseNode	*m_pChildHead;
	CParseNode	*m_pChildTail;
	CParseNode	*m_pNext;
	CParseNode	*m_pPrevious;
	CParseNode	*m_pParentNode;
};

class CParseNodeTableBlock
{
public:
	CParseNodeTableBlock(long NodeCount);
	~CParseNodeTableBlock();

	CParseNode				*m_pParseNodeTable;
	CParseNodeTableBlock	*m_pNextNodeBlock;
	CParseNodeTableBlock	*m_pPreviousNodeBlock;
	long					 m_NodeCount;

	long					 m_FirstNodeID;
};

class CParseNodeManager
{
public:
	CParseNodeManager(int ParseNodeCount=128);
	~CParseNodeManager();
	bool	InitOK() {return(this->m_bInitOK);};

	void	Reset();

//	int			AddNode(int NodeOperator, char *pszStringStart, char *pszStringFinish);

	CParseNode	*TokenPrep(int NodeOperator, char *pszTokenStringStart);
	CParseNode	*TokenPost(CParseNode *pNode, CParseNode *pParentNode, char *pszTokenStringEnd);

	CParseNode	*FindFirstChild(int NodeOperator, const CParseNode *pStartNode, bool bRecurseChildren) const;	// Given a node, searches child, then optionally all children for a match
	CParseNode	*FindFirst(int NodeOperator, const CParseNode *pStartNode, bool bRecurseChildren) const;		// Given a node, searches the node and all linked nodes, and optionally all children for first match
	CParseNode	*FindFirst(int NodeOperator, bool bRecurseChildren=true) const;							// Given no node, searches all linked nodes, and optionally all children for first match
	CParseNode	*FindNext(int NodeOperator, const CParseNode *pStartNode, bool bRecurseChildren) const;			// Given a node, searches optionally the node children then all linked nodes, and optionally all children for first match

	CParseNode	*GetRootNode();

	void		SetSingleNodeReturn(int Node, bool nodeMode=true);

	void		NodeReturnSet(int NodeSet);

	int		GetParseNodeTypeCount();
	long		GetNodeCount();
	CParseNode	*GetNode(int index);

	CParseNode	*InjectChildNode(CParseNode *pNodeParent, int NodeOperator, char *pszNodeString, CParseNode *pInsertBefore = NULL /* NULL to insert at end of current child list */);


private:
	bool		m_bInitOK;

#ifdef NLM
	unsigned int m_nRelinquishCPU;
#endif

	int			m_ParseNodeCount;
	bool		*m_FireNode;	// true if the node is to report its information
	CParseNodeTableBlock	*m_pFirstNodeTable;
	CParseNodeTableBlock	*m_pActiveNodeTable;

	long		m_CurrentNodeCount;
	long		m_CurrentNodeArraySize;
	long		m_NodeBlockExpandSize;			// If we need more memory, this is how many nodes we will expand the array by
	long		m_NodeBlockExpandIncrement;		// Each time the array needs expansion, we grow the expansion amount to allow for increasing memory requirements without continual small block allocation
};

// Derive specific parser classes from this object
class CParser
{
public:
	CParser();	// Validation only does not store a tree.  This is an easy way to quick scan and return position errors
	~CParser();

	void	Reset();

	char	*GetParserError();				// Returns the last set error from the parse
	char	*GetParserErrorContext();		// Grabs some text from around the last error for error context
	long	GetParserErrorContextOffset();	// Get the position of the error itself from within the error context
	bool	CalculateErrorPositions();		// Re-parses source and computes line numbers / column postitions of parse errors.
											// Assumes <LF> is used to seperate lines
											// Returns false if there is no error
	long	GetErrorPosition();
	long	GetErrorLine();
	long	GetErrorColumn();

	void	DumpParseTreeToDisk(char *pszOutputFile, bool bDumpText);
	void	DumpNodeToDisk(FILE *hFile, CParseNode *pNode, bool bDumpText = false, int nTreeDepth = 0);

	// String helper functions that can be used without needing to run TerminateStrings
	int		node_strcmp(CParseNode *pNode, char *pszString);
	int		node_stricmp(CParseNode *pNode, char *pszString);
	void	node_strcat(CParseNode *pNode, char *pszString);

	//	String manipulation operations
	void	TerminateString(CParseNode *pNode);
	void	UnTerminateString(CParseNode *pNode);
	void	TerminateStrings(int nNodeType, CParseNode *pStartNode);
	void	TerminateStrings(int nNodeType);	// zero delimit all strings in the tree with the given node type  This will impact on whatever follows the element
												// string, but if only certain types of data are to be used and the grammar defines that the required data is
												// delimited by other non-required objects, then this can make handling the data much easier from the client side
	void	TerminateStringsToDepth(int nMaxDepth, int nNodeType, CParseNode *pStartNode);
	void	TerminateStringsToDepth(int nMaxDepth, int nNodeType);

	void	UnTerminateStrings(int nNodeType, CParseNode *pStartNode);
	void	UnTerminateStrings(int nNodeType);
	void	UnTerminateStringsToDepth(int nMaxDepth, int nNodeType, CParseNode *pStartNode);
	void	UnTerminateStringsToDepth(int nMaxDepth, int nNodeType);

	bool	RewriteData(char *pszFilename, CParseNode *pTopNode = NULL);	// Rewrites the parse tree out to a target file
	bool	RewriteData(FILE *hFileHandle, CParseNode *pTopNode = NULL);
	bool	RewriteData(char **pszFilename, CParseNode *pTopNode = NULL);	// Rewrites the parse tree out to a string

public:
	//	Parser node access
	CParseNode *GetRootNode();
	CParseNode	*FindFirstChild(int NodeOperator, const CParseNode *pStartNode, bool bRecurseChildren=true) const;
	CParseNode	*FindFirst(int NodeOperator, const CParseNode *pStartNode, bool bRecurseChildren) const; // Given a node, searches the node and all linked nodes, and optionally all children for first match
	CParseNode	*FindFirst(int NodeOperator, bool bRecurseChildren=true) const;					// Given no node, searches all linked nodes, and optionally all children for first match
	CParseNode	*FindNext(int NodeOperator, const  CParseNode *pStartNode, bool bRecurseChildren) const; // Given a node, searches optionally the node children then all linked nodes, and optionally all children for first match

	bool ParseNodeManagerExists() const;

	void	SetReturnNodes(int nNodes, ...);
	void	SetReturnNode(int nNodes, bool bState);
	void	SetAllReturnNodes(bool bMode);
	int		GetParseNodeTypeCount();

	CParseNode *InjectChildNode(CParseNode *pNodeParent, int NodeOperator, char *pszNodeString, CParseNode *pInsertBefore = NULL);

protected:
	CParseNode *TokenParsePrep(int nNodeType, char *pszTokenStart);
	void	TokenParsePost(CParseNode *pNode, CParseNode *pParentNode, char *pszTokenEnd, int nNodeType=0, char *pszTokenStart=0);
	void	SetParserError(char *pszErrorPos, const char* const pszError);	// Max length 255 bytes.  Anything more will be truncated
	void	SetParserError(char *pszErrorPos, int nError);	// Set last parse error for lookup at end of parse
	void	ClearParserError();

	void	InitParser(char *pszSource, size_t nSourceLength=0, const char **ppszErrorStrings=0);	// nSourceLength is required when a delimiter is not defined
	void	SetDataSource(char *pszSource, size_t nSourceLength);

	CParseNodeManager *InitParseTree(int nTokenCount);
	char	*GetDataSource();
	char	*GetDataEnd();

private:
	CParseNodeManager *m_pParseNodeManager;	// The node manager
	int		m_nErrorIndex;									// Storage for error positioning during parsing
	char	*m_pszErrorPos;									// Pointer to where the last error occurred
	char	m_szError[MAX_ERROR_STRING_SIZE+1];				// Write the last error here.  To report multple errors, write your own handler for this
	char	m_szErrorContext[ERROR_CONTEXT_STRING_SIZE+1];	// A block of data from up to 16 bytes prior to and 48 bytes after a set error, for human readability and debugging
	long	m_nErrorContextErrorOffset;						// Within the error context this is the position that the error itself begins
	long	m_nErrorLine;									// Call CalculateErrorPositions() to fill make this valid
	long	m_nErrorColumn;									// Call CalculateErrorPositions() to fill make this valid
	char	*m_pszSourceData;								// The source of the data to parse
	char	*m_pszSourceDataEnd;							// Where the data ends - required if terminating chars aren't part of the spec
	size_t	m_nDataLength;									// How much data to parse.  May be required for data types that have no particular end of data delimiter

	const char	**m_ppszErrorStrings;						// String error reports need this setup to return error codes
};

#endif //GUARD_PARSENODEMANAGER_H_1378285311l


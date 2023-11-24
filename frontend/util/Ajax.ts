import axios from 'axios';

export const getRequestSender = (url: string) => axios.get(url).then((res) => res.data);
export const postRequestSender = <TRequestBody,>(url: string, { arg }: { arg: TRequestBody }) => axios.post(url, { ...arg });
